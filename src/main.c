#include "main.h"
#include "hid.h"
#include "descriptors.h"
#include "mapping.h"


void registers_to_report(uint8_t *report) {
    const uint8_t num_registers = 7;

    uint8_t *mapping;

    if (kb_Data[2] & kb_Alpha) {
        mapping = SCAN_CODES_ALPHA;
    } else if (kb_Data[1] & kb_2nd) {
        mapping = SCAN_CODES_2ND;
    } else {
        mapping = SCAN_CODES;
    }

    uint8_t report_index = 2; // skip modifier byte and reserved byte to get to keycodes
    for (uint8_t i = 0; i < num_registers * 8; ++i) {
        uint8_t offset = (i >> 3) + 1;
        uint8_t bit = i & 0x7;

        uint8_t is_pressed = (kb_Data[offset] >> bit) & 1;

        if (!is_pressed) {
            continue;
        }

        uint8_t scan_code = mapping[i];

        if (scan_code) {
            report[report_index] = scan_code;
            report_index++;
        }
    }

    // make sure we zero out the rest
    for (uint8_t i = report_index; i < 8; ++i) {
        report[i] = 0;
    }

    for (uint8_t i = 0; i < 8; ++i) {
        uint8_t mask = 1 << i;

        uint8_t key = MODIFIER_MAP[i];

        if (key == 0xFF) {
            continue; // unused modifier
        }

        uint8_t offset = (key >> 3) + 1;
        uint8_t bit = key & 0x7;

        uint8_t is_pressed = (kb_Data[offset] >> bit) & 1;

        if (is_pressed) {
            report[0] |= mask;
        } else {
            report[0] &= ~mask;
        }
    }
}


int main(void) {
    os_ClrHome();

    printf("Welcome to TIMPANI\n");

    printf("Press MODE to exit\n");

    usb_Init(usb_event, NULL, &descriptors, USB_DEFAULT_INIT_FLAGS);

    static uint8_t last_report[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    while (1) {
        kb_Scan();

        if (kb_Data[1] & kb_Mode) {
            break;
        }

        static uint8_t report[] = {
            0x00, // modifier byte
            0x00, // reserved byte
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        };

        registers_to_report(report);

        if (memcmp(report, last_report, 8) != 0) {
            send_report(report);
            memcpy(last_report, report, 8);
        }

        usb_HandleEvents();
    }

    printf("USB cleaning up\n");

    usb_Cleanup();

    return 0;
}
