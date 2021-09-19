#include "main.h"

#define MANUFACTURER_STRING { L'T', L'e', L'x', L'a', L's', L' ', L'I', L'n', L's', L't', L'r', L'u', L'm', L'e', L'n', L't', L's', L' ', L'I', L'n', L'c', L'o', L'r', L'p', L'o', L'r', L'a', L't', L'e', L'd' }
#define PRODUCT_STRING { L'T', L'I', L'-', L'8', L'4', L' ', L'P', L'l', L'u', L's', L' ', L'C', L'E' }
#define SERIAL_STRING { L'0', L'0', L'0', L'0', L'0', L'0', L'0', L'0' }

const static usb_string_descriptor_t langids = {
    .bLength = sizeof(langids) + sizeof(wchar_t) * 1,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = {
        [0] = 0x0409u,
    },
};

const static usb_string_descriptor_t manufacturer = {
    .bLength = sizeof(manufacturer) + sizeof(wchar_t) * 30,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = MANUFACTURER_STRING,
};
const static usb_string_descriptor_t product = {
    .bLength = sizeof(manufacturer) + sizeof(wchar_t) * 13,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = PRODUCT_STRING,
};
const static usb_string_descriptor_t serial = {
    .bLength = sizeof(manufacturer) + sizeof(wchar_t) * 8,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = SERIAL_STRING,
};


const static struct configuration1 {
    usb_configuration_descriptor_t configuration;
    struct configuration1_interface0 {
        usb_interface_descriptor_t interface;
        hid_descriptor_t hid;
        usb_endpoint_descriptor_t endpoints[1];
    } interface0;
} configuration1 = {
    .configuration = {
        .bLength = sizeof(configuration1.configuration),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(configuration1),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = USB_CONFIGURATION_ATTRIBUTES | USB_SELF_POWERED | USB_NO_REMOTE_WAKEUP,
        .bMaxPower = 500u / 2u,
    },
    .interface0 = {
        .interface = {
            .bLength = sizeof(configuration1.interface0.interface),
            .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
            .bInterfaceNumber = 0,
            .bAlternateSetting = 0,
            .bNumEndpoints = 1,
            .bInterfaceClass = 3, // HID interface class
            .bInterfaceSubClass = 1, // Boot Interface
            .bInterfaceProtocol = 1, // Keyboard
            .iInterface = 0,
        },
        .hid = {
            .bLength = sizeof(hid_descriptor_t),
            .bDescriptorType = 0x21,
            .bcdHID = 0x0110,
            .bCountryCode = 0,
            .bNumDescriptors = 1,
            .bDescriptorType2 = 0x22,
            .wDescriptorLength = sizeof(report_descriptor)
        },
        .endpoints = {
            [0] = {
                .bLength = sizeof(configuration1.interface0.endpoints[0]),
                .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
                .bEndpointAddress = 0x81,  // IN
                .bmAttributes = USB_INTERRUPT_TRANSFER,
                .wMaxPacketSize = 8,
                .bInterval = 8,
            },
        },
    },
};

const static usb_configuration_descriptor_t *configurations[] = {
    [0] = &configuration1.configuration,
};

const static usb_device_descriptor_t device = {
    .bLength = sizeof(device),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200u,
    .bDeviceClass = USB_HID_CLASS,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 0x40,
    .idVendor = 0x0451u, // Texas Instruments
    // .idProduct = 0xE008u, // TI-84 Plus CE
    .idProduct = 0xE009u, // Add one to the PID to ensure unique
    .bcdDevice = 0x0100u,
    .iManufacturer = 2,
    .iProduct = 3,
    .iSerialNumber = 4,
    .bNumConfigurations = 1,
};


const static usb_string_descriptor_t *strings[] = {
    [0] = &langids,
    [1] = &manufacturer,
    [2] = &product,
    [3] = &serial,
};

const static usb_standard_descriptors_t descriptors = {
    .device = &device,
    .configurations = configurations,
    .langids = &langids,
    .numStrings = 4,
    .strings = strings,
};
