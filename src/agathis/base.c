// SPDX-License-Identifier: GPL-3.0-or-later

#include "base.h"

#include <stdio.h>

#if defined(__AVR__)
#include <avr/wdt.h>
#elif defined(__linux__)
#include "../sim/state.h"
#include "../sim/misc.h"
#endif

#include "defs.h"
#include "config.h"
#include "../hw/gpio.h"
#include "../hw/storage.h"
#include "../platform/platform.h"

AG_MC_STATE_t MOD_STATE = {.caps_hw_ext = 0, .caps_hw_int = 0, .caps_sw = 0,
                           .last_err = 0, .type = 0,
                           .mfr_name = "", .mfr_pn = "", .mfr_sn = "",
                           .i5_nom = 0.1f,  .i5_cutoff = 0.12f, .i3_nom = 1.0f, .i3_cutoff = 1.5f,
                          };

AG_RMT_MC_STATE_t REMOTE_MODS[MC_MAX_CNT] = {
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
    {.mac = {0, 0}, .caps = 0, .last_err = 0, .last_seen = -1},
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
    return SIM_STATE.id;
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

void ag_add_remote_mod(const uint32_t *mac, uint8_t caps) {
    int idx_free = -1;
    for (int i = 0 ; i < MC_MAX_CNT; i ++) {
        if ((REMOTE_MODS[i].last_seen == -1) && (idx_free == -1)) {
            idx_free = i;
        }
        if ((REMOTE_MODS[i].mac[0] == mac[0]) && (REMOTE_MODS[i].mac[1] == mac[1])) {
            REMOTE_MODS[i].caps = caps;
            REMOTE_MODS[i].last_seen = 0;
            return;
        }
    }

    if (idx_free >= 0) {
        REMOTE_MODS[idx_free].mac[0] = mac[0];
        REMOTE_MODS[idx_free].mac[1] = mac[1];
        REMOTE_MODS[idx_free].caps = caps;
        REMOTE_MODS[idx_free].last_seen = 0;
    } else {
        printf("CANNOT add MC - too many\n");
    }
}

void ag_upd_remote_mods(void) {
    for (int i = 0 ; i < MC_MAX_CNT; i ++) {
        if (REMOTE_MODS[i].last_seen == -1) {
            continue;
        }
        if (REMOTE_MODS[i].last_seen > MC_MAX_AGE) {
            REMOTE_MODS[i].mac[0] = 0;
            REMOTE_MODS[i].mac[1] = 0;
            REMOTE_MODS[i].caps = 0;
            REMOTE_MODS[i].last_err = AG_ERR_NONE;
            REMOTE_MODS[i].last_seen = -1;
        } else {
            REMOTE_MODS[i].last_seen += 1;
        }
    }
}

void ag_upd_alarm(void) {
    int nm = 0;
    for (int i = 0 ; i < MC_MAX_CNT; i ++) {
        if ((REMOTE_MODS[i].caps & AG_CAP_SW_TMC) != 0) {
            nm += 1;
        }
    }
    if ((MOD_STATE.caps_sw & AG_CAP_SW_TMC) != 0) {
        nm += 1;
    }
    if (nm > 1) {
        MOD_STATE.last_err = AG_ERR_MULTI_MASTER;
        return;
    }
    MOD_STATE.last_err = AG_ERR_NONE;
}

void ag_reset(void) {
#if defined(__AVR__)
    printf("reset\n");
    wdt_enable(WDTO_15MS);
#elif defined(__XC16__)

#endif
}

void ag_init(void) {
#if MOD_HAS_EEPROM
    MOD_STATE.caps_hw_int = AG_CAP_INT_EEPROM;
#endif

#if MOD_HAS_PWR
    MOD_STATE.caps_hw_ext |= AG_CAP_EXT_PWR;
#endif
#if MOD_HAS_CLK
    MOD_STATE.caps_hw_ext |= AG_CAP_EXT_CLK;
#endif
#if MOD_HAS_1PPS
    MOD_STATE.caps_hw_ext |= AG_CAP_EXT_1PPS;
#endif
#if MOD_HAS_JTAG
    MOD_STATE.caps_hw_ext |= AG_CAP_EXT_JTAG;
#endif
#if MOD_HAS_USB
    MOD_STATE.caps_hw_ext |= AG_CAP_EXT_USB;
#endif
#if MOD_HAS_PCIE
    MOD_STATE.caps_hw_ext |= AG_CAP_EXT_PCIE;
#endif

//    MOD_STATE.addr_d = p_gpio_addr_d();
//    MOD_STATE.addr_i2c = (I2C_OFFSET + MOD_STATE.addr_d);
//    if (MOD_STATE.addr_d == 0) {
//        MOD_STATE.caps_hw_ext |= AG_CAP_EXT_TMC;
//        MOD_STATE.addr_u = (MOD_STATE.addr_d + 1);
//        //MOD_STATE.addr_i2c = (I2C_OFFSET + 1); // TODO: for test, remove on release
//    } else if (MOD_STATE.addr_d == (MC_MAX_CNT - 1)) {
//        MOD_STATE.caps_hw_ext |= AG_CAP_EXT_TMC;
//        MOD_STATE.addr_u = 1;
//    } else {
//        MOD_STATE.addr_u = (MOD_STATE.addr_d + 1);
//    }
//    p_gpio_addr_u(MOD_STATE.addr_u);
//
//    printf("%s caps: 0x%02X / 0x%02X\n", PREFIX_MC,
//           MOD_STATE.caps_hw_ext, MOD_STATE.caps_hw_int);
//    printf("%s addr: 0x%02X / 0x%02X (0x%02X)\n", PREFIX_MC,
//           MOD_STATE.addr_d, MOD_STATE.addr_u, MOD_STATE.addr_i2c);

#if MOD_HAS_EEPROM
    stor_restore_state();
#endif
    MOD_STATE.last_err = AG_ERR_NONE;
}

float ag_get_I5_NOM(void) {
#if defined(__AVR__)
    return 0.0f;
#elif defined(__XC16__)
    return 0.0f;
#else
    return getValue_random((MOD_STATE.i5_nom * 0.7f), 5);
#endif
}

float ag_get_I3_NOM(void) {
#if defined(__AVR__)
    return 0.0f;
#elif defined(__XC16__)
    return 0.0f;
#else
    return getValue_random((MOD_STATE.i3_nom * 0.5f), 5);
#endif
}
