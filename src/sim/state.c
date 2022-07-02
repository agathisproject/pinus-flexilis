// SPDX-License-Identifier: GPL-3.0-or-later

#include "state.h"

SIM_STATE_t SIM_STATE = {.eeprom_path = "EEPROM.BIN",
                         .config_path = "config.ini",
                         .sim_flags = 0,
                         .id = 0, .mac = {0x02, 0xd0, 0xd0, 0, 0, 0}
                        };
