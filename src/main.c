typedef struct usb_state usb_state_t;
#define usb_callback_data_t usb_state_t

#include <tice.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <usbdrvce.h>


struct usb_state {
    usb_device_t device;
    usb_endpoint_t in, out;
    uint8_t type;
};

usb_string_descriptor_t make_string_descriptor(const char *str) {
    usb_string_descriptor_t* ret;

    ret = malloc(sizeof(usb_string_descriptor_t) + strlen(str) * sizeof(wchar_t));
    ret->bLength = sizeof(usb_string_descriptor_t) + strlen(str) * sizeof(wchar_t);
    ret->bDescriptorType = USB_STRING_DESCRIPTOR;

    for (uint8_t i = 0; i < strlen(str); i++) {
        ret->bString[i] = str[i];
    }

    return *ret;
}

usb_standard_descriptors_t get_serial_descriptors() {
    // Descriptor for a Serial device
    const usb_device_descriptor_t device_descriptor = {
        .bLength = sizeof(usb_device_descriptor_t),
        .bDescriptorType = USB_DEVICE_DESCRIPTOR,
        .bcdUSB = 0x0200,
        .bDeviceClass = USB_MISCELLANEOUS_CLASS,
        .bDeviceSubClass = 2, // Subclass Common
        .bDeviceProtocol = 1, // Protocol: IAD
        .bMaxPacketSize0 = 64, // TODO: What is this?
        .idVendor = 0x0451, // Pretend to be a TUSB3410
        .idProduct = 0x3410, // https://www.ti.com/product/TUSB3410#product-details##params
        .bcdDevice = 0x0100,
        .iManufacturer = 1,
        .iProduct = 2,
        .iSerialNumber = 3,
        .bNumConfigurations = 1,
    };

    const usb_configuration_attributes_t configuration_attributes = USB_BUS_POWERED;

    const usb_configuration_descriptor_t configuration_descriptor = {
        .bLength = sizeof(usb_configuration_descriptor_t),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(usb_configuration_descriptor_t),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = configuration_attributes,
        .bMaxPower = 100, // 100mA
    };

    const usb_configuration_descriptor_t* configurations[] = {&configuration_descriptor};

    const char langid_string[] = { 0x04, 0x09, 0x00 };  // English
    const usb_string_descriptor_t langid_descriptor = make_string_descriptor(langid_string);
    const usb_string_descriptor_t langids[] = {langid_descriptor};

    const usb_standard_descriptors_t descriptors = {
        .device = &device_descriptor,
        .configurations = configurations,
        .langids = langids,
        .numStrings = 1,
        .strings = NULL, // TODO: What is this?
    };

    return descriptors;
}


usb_error_t handle_bulk_out(usb_endpoint_t endpoint, usb_transfer_status_t status, size_t transferred, usb_transfer_data_t *data) {
    if (status == USB_TRANSFER_COMPLETED) {
        return usb_ScheduleTransfer(endpoint, data, 512, NULL, data);
    }
    free(data);
    return USB_SUCCESS;
}


enum usb_error usb_event_callback(usb_event_t event, void *data, usb_callback_data_t *state) {
    printf("EVENT %d\n", event);

    usb_device_t host;

    switch (event) {
        case USB_ROLE_CHANGED_EVENT:
            printf("USB_ROLE_CHANGED_EVENT\n");
            break;

        case USB_DEVICE_DISCONNECTED_EVENT:
            printf("USB_DEVICE_DISCONNECTED_EVENT\n");
            if (state->device == data) {
                state->device = NULL;
                state->in = state->out = NULL;
            }
            break;

        case USB_DEVICE_CONNECTED_EVENT:
            printf("USB_DEVICE_CONNECTED_EVENT\n");
            if (!(usb_GetRole() & USB_ROLE_DEVICE)) {
                printf("ERROR: Not a device\n");
                return usb_ResetDevice(data);
            }
            break;

        case USB_DEVICE_ENABLED_EVENT:
            printf("USB_DEVICE_ENABLED_EVENT\n");
            state->device = data;
            break;

        case USB_DEVICE_DISABLED_EVENT:
            printf("USB_DEVICE_DISABLED_EVENT\n");
            break;

        case USB_DEFAULT_SETUP_EVENT:
            printf("USB_DEFAULT_SETUP_EVENT\n");

            const usb_control_setup_t clear_out_endpoint_halt_setup = {
                .bmRequestType = USB_HOST_TO_DEVICE | USB_STANDARD_REQUEST | USB_RECIPIENT_ENDPOINT,
                .bRequest = USB_CLEAR_FEATURE_REQUEST,
                .wValue = USB_ENDPOINT_HALT_FEATURE,
                .wIndex = 0x02,
                .wLength = 0,
            };

            const usb_control_setup_t *setup = data;

            host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);

            if (!host) {
                printf("ERROR: No host found\n");
                return USB_ERROR_NO_DEVICE;
            }

            if (!memcmp(setup, &clear_out_endpoint_halt_setup, sizeof(usb_control_setup_t))) {
                printf("CLEAR_FEATURE_REQUEST\n");
                usb_endpoint_t out_endpoint = usb_GetDeviceEndpoint(host, 0x02);

                if (!out_endpoint) {
                    printf("ERROR: No endpoint\n");
                    return USB_ERROR_SYSTEM;
                }

                void *buffer;
                if (!(buffer = malloc(512))) {
                    printf("ERROR: Out of memory\n");
                    return USB_ERROR_NO_MEMORY;
                }

                handle_bulk_out(out_endpoint, USB_TRANSFER_COMPLETED, 0, buffer); // TODO: implement
            } else if ((setup->bmRequestType & ~USB_DEVICE_TO_HOST) == (USB_VENDOR_REQUEST | USB_RECIPIENT_DEVICE) && !setup->bRequest && !setup->wValue && !setup->wIndex) {
                printf("VENDOR_REQUEST\n");

                usb_endpoint_t default_contorl_endpoint = usb_GetDeviceEndpoint(host, 0x00);

                if (!default_contorl_endpoint) {
                    printf("ERROR: No endpoint\n");
                    return USB_ERROR_SYSTEM;
                }

                if (setup->bmRequestType & USB_DEVICE_TO_HOST) {
                    printf("Huh, what do I do here? 01\n");
                } else {
                    printf("Huh, what do I do here? 02\n");
                }
                return USB_IGNORE;
            }
            break;

        case USB_HOST_CONFIGURE_EVENT:
            printf("USB_HOST_CONFIGURE_EVENT\n");

            host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);

            if (!host) {
                printf("ERROR: No host found\n");
                return USB_ERROR_NO_DEVICE;
            }

            usb_endpoint_t in = usb_GetDeviceEndpoint(host, 0x81);
            usb_endpoint_t out = usb_GetDeviceEndpoint(host, 0x02);

            if (!in || !out) {
                printf("ERROR: No endpoint\n");
                return USB_ERROR_SYSTEM;
            }

            break;
    }

    return USB_SUCCESS;
}


void handle_device(usb_state_t *state) {
    // printf("What do I do here? 03");
}


int main(void) {
    usb_state_t state;

    os_ClrHome();

    printf("Welcome to TIMPANI\n");

    printf("Initializing USB\n");

    const usb_standard_descriptors_t descriptors = get_serial_descriptors();
    usb_Init(usb_event_callback, &state, &descriptors, USB_DEFAULT_INIT_FLAGS);

    printf("USB initialized\n");

    while (usb_WaitForInterrupt() == USB_SUCCESS && !os_GetCSC()) {
        handle_device(&state);
    }

    printf("USB cleaning up\n");

    usb_Cleanup();

    return 0;
}