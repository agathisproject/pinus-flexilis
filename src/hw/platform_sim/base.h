// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BASE_JZCR9N5N9ACAK9FF
#define BASE_JZCR9N5N9ACAK9FF

#include <stdint.h>

/**
 * @brief send RBG code to LED
 *
 * @param code uint32_t 0x00rrggbb where only the last 3 bytes are sent on the wire
 * byte 2 is the blue value
 * byte 1 is the green value
 * byte 0 is the red value
 */
void gpio_RGB_send(uint32_t code);

#endif /* BASE_JZCR9N5N9ACAK9FF */
