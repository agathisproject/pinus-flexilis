// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STATE_7Y7NKE833U9TJQAD
#define STATE_7Y7NKE833U9TJQAD
/** @file */

#include <mqueue.h>
#include <stdint.h>

#define SIM_PATH_LEN 32
#define SIM_MQ_PREFIX "agathis_"

#define SIM_FLAG_NO_CONSOLE 0x00000001

typedef struct {
    char eeprom_path[SIM_PATH_LEN];
    char config_path[SIM_PATH_LEN];
    mqd_t msg_queue;
    uint32_t sim_flags;
    uint8_t id;
    uint8_t mac[6];
} SIM_STATE_t;

extern SIM_STATE_t SIM_STATE;

#endif /* STATE_7Y7NKE833U9TJQAD */
