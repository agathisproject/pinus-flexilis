// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdlib.h>

#include "../platform.h"
#include "state.h"

void platform_init(void) {

}

void hw_GetID(uint8_t *mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = SIM_STATE.mac[i];
    }
}

void hw_GetIDCompact(uint32_t *mac) {
    // *INDENT-OFF*
    mac[1] = ((uint32_t) SIM_STATE.mac[5] << 16) | ((uint32_t) SIM_STATE.mac[4] << 8) | SIM_STATE.mac[3];
    mac[0] = ((uint32_t) SIM_STATE.mac[2] << 16) | ((uint32_t) SIM_STATE.mac[1] << 8) | SIM_STATE.mac[0];
    // *INDENT-ON*
}

void gpio_SetRGB(uint32_t code) {

}

float getValue_random(float val, int maxPercent) {
    float tmp = (float)((rand() % (2 * maxPercent)) - maxPercent) / 100.0f;
    return val * (1.0f + tmp);
}

float hw_GetTemperature(void) {
    return getValue_random(25.0, 20);
}
