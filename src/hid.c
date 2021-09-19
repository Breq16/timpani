#include "hid.h"

usb_error_t usb_event(usb_event_t event, void* event_data, void* user_data) {

    if (event == USB_DEFAULT_SETUP_EVENT) {
        // Send the descriptors when requested

        usb_control_setup_t* setup = event_data;

        usb_endpoint_t control = usb_GetDeviceEndpoint(
            usb_FindDevice(NULL, NULL, USB_SKIP_HUBS),
            0
        );

        if (
            setup->bmRequestType == (
                USB_DEVICE_TO_HOST |
                USB_STANDARD_REQUEST |
                USB_RECIPIENT_INTERFACE
            ) &&
            setup->bRequest == USB_GET_DESCRIPTOR_REQUEST
        ) {
            uint8_t descriptor = setup->wValue >> 8;

            if (descriptor == 0x21) {
                // HID descriptor

                static const hid_descriptor_t hid_descriptor = {
                    .bLength = sizeof(hid_descriptor_t),
                    .bDescriptorType = 0x21,
                    .bcdHID = 0x0110,
                    .bCountryCode = 0,
                    .bNumDescriptors = 1,
                    .bDescriptorType2 = 0x22,
                    .wDescriptorLength = sizeof(report_descriptor)
                };

                usb_error_t error = usb_ScheduleTransfer(
                    control,
                    (void *)&hid_descriptor,
                    sizeof(hid_descriptor),
                    NULL,
                    NULL
                );

                if (error == USB_SUCCESS) {
                    error = USB_IGNORE;
                }
                return error;

            } else if (descriptor == 0x22) {
                // Report descriptor

                usb_error_t error = usb_ScheduleTransfer(
                    control,
                    (void *)&report_descriptor,
                    sizeof(report_descriptor),
                    NULL,
                    NULL
                );

                if (error == USB_SUCCESS) {
                    error = USB_IGNORE;
                }
                return error;
            } else {
                return USB_SUCCESS;
            }
        }
    }

    return USB_SUCCESS;
}

void send_report(uint8_t* report) {
    usb_endpoint_t interrupt_endpoint = usb_GetDeviceEndpoint(
        usb_FindDevice(NULL, NULL, USB_SKIP_HUBS),
        0x81
    );

    if (!interrupt_endpoint) {
        return;
    }

    usb_ScheduleTransfer(
        interrupt_endpoint,
        report,
        8,
        NULL,
        NULL
    );
}

