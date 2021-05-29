// SPDX-License-Identifier: GPL-3.0-or-later

#include "base.h"

#include <stdio.h>
#if defined(__AVR__)
#include <avr/wdt.h>
#include "../hw/gpio.h"
#include "../platform.h"
#elif defined (__XC16__)
#include "../hw/gpio.h"
#include "../platform.h"
#endif

AG_MC_STATE_t MOD_STATE = {0, 0, 0,
                           0, 0, 0,
                           0, 0xFFFF
                          };

AG_MC_SCAN_INFO_t REMOTE_MODS[MC_MAX_CNT - 1] = {
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
    {MC_NOT_PRESENT, 0, 0},
};

#if defined(__XC16__)
SemaphoreHandle_t xSemaphore_MMC = NULL;
#endif

#if defined(__AVR__) || defined(__XC16__)
uint8_t p_gpio_addr_d(void) {
    uint8_t res = (uint8_t) ((gpio_get(PIN_D_ADDR2) << 2) +
                             (gpio_get(PIN_D_ADDR1) << 1) +
                             gpio_get(PIN_D_ADDR0));
    return res;
}
#elif defined(__linux__)
uint8_t p_gpio_addr_d(void) {
    return 1;
}
#endif

#if defined(__AVR__) || defined(__XC16__)
void p_gpio_addr_u(uint8_t addr) {
    gpio_set(PIN_U_ADDR0, (addr & 0x01));
    gpio_set(PIN_U_ADDR1, ((addr >> 1) & 0x01));
    gpio_set(PIN_U_ADDR2, ((addr >> 2) & 0x01));
}
#elif defined(__linux__)
void p_gpio_addr_u(uint8_t addr) {

}
#endif

void ag_init(void) {
    MOD_STATE.flags = AG_FLAG_EEPROM;
    MOD_STATE.caps = (AG_CAP_CLK | AG_CAP_1PPS | AG_CAP_JTAG);

    MOD_STATE.addr_d = p_gpio_addr_d();
    MOD_STATE.addr_i2c = (I2C_OFFSET + MOD_STATE.addr_d);
    if (MOD_STATE.addr_d == 0) {
        MOD_STATE.flags |= AG_FLAG_TMC;
        MOD_STATE.addr_u = (MOD_STATE.addr_d + 1);
        //MOD_STATE.addr_i2c = (I2C_OFFSET + 1); // TODO: for test, remove on release
    } else if (MOD_STATE.addr_d == (MC_MAX_CNT - 1)) {
        MOD_STATE.flags |= AG_FLAG_TMC;
        MOD_STATE.addr_u = 1;
    } else {
        MOD_STATE.addr_u = (MOD_STATE.addr_d + 1);
    }
    p_gpio_addr_u(MOD_STATE.addr_u);

    MOD_STATE.last_err = 0;
    printf("%s addr: 0x%02X / 0x%02X (0x%02X)\n", PREFIX_MC,
           MOD_STATE.addr_d, MOD_STATE.addr_u, MOD_STATE.addr_i2c);
}
