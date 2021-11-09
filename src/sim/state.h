// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STATE_7Y7NKE833U9TJQAD
#define STATE_7Y7NKE833U9TJQAD
/** @file */

#include <stdint.h>

#define SIM_PATH_LEN 64

typedef struct {
    char eeprom_path[SIM_PATH_LEN];
    char config_path[SIM_PATH_LEN];
    uint8_t addr_d;
} SIM_STATE_t;

extern SIM_STATE_t SIM_STATE;

#endif /* STATE_7Y7NKE833U9TJQAD */
