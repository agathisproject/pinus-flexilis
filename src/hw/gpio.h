// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GPIO_L99W73D5XHHCJZAZ
#define GPIO_L99W73D5XHHCJZAZ
/** @file */

#include <stdint.h>

typedef enum {
    PIN_0 = 0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_RA0,
    PIN_RA1,
    PIN_RC3,
    PIN_RC4,
    PIN_RC5,
    PIN_RC7,
    PIN_NONE,
} PIN_ID_t;

typedef enum {
    PIN_TYPE_IN = 0,
    PIN_TYPE_OUT,
    PIN_TYPE_TRISTATE,
} PIN_TYPE_t;

void gpio_cfg(PIN_ID_t pinId, PIN_TYPE_t type);

void gpio_set(PIN_ID_t pinId, uint8_t val);

uint8_t gpio_get(PIN_ID_t pinId);

#endif /* GPIO_L99W73D5XHHCJZAZ */
