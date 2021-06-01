// SPDX-License-Identifier: GPL-3.0-or-later

#include "base.h"

#include <stdio.h>
#if defined(__AVR__)
#include <avr/wdt.h>
#include "../hw/gpio.h"
#include "../platform.h"
#elif defined(__XC16__)
#include "../hw/gpio.h"
#include "../platform.h"
#elif defined(__linux__) || defined(__MINGW64__)
#include "stdlib.h"
#include "sys/stat.h"
#include "../sim/state.h"
#endif
#include "config.h"

AG_MC_STATE_t MOD_STATE = {0, 0, 0,
                           0, 0, 0, 0xFFFF,
                           "", "", ""
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
#elif defined(__linux__) || defined(__MINGW64__)
uint8_t p_gpio_addr_d(void) {
    return SIM_STATE.addr_d;
}
#endif

#if defined(__AVR__) || defined(__XC16__)
void p_gpio_addr_u(uint8_t addr) {
    gpio_set(PIN_U_ADDR0, (addr & 0x01));
    gpio_set(PIN_U_ADDR1, ((addr >> 1) & 0x01));
    gpio_set(PIN_U_ADDR2, ((addr >> 2) & 0x01));
}
#elif defined(__linux__) || defined(__MINGW64__)
void p_gpio_addr_u(uint8_t addr) {

}
#endif

#if defined(__AVR__) || defined(__XC16__)
void p_restore_state(void) {

}
#elif defined(__linux__) || defined(__MINGW64__)
void p_restore_state(void) {
    const char *fName = "EEPROM.BIN";
    FILE *fp;
    struct stat s;

    if (stat(fName, &s) == -1) {
        fp = fopen(fName, "wb");
        if (!fp) {
            perror("CANNOT create file");
            exit(EXIT_FAILURE);
        }
        for (unsigned int i = 0; i < 1024; i ++) {
            fputc(0xFF, fp);
        }
        fclose(fp);
        return;
    }

    fp = fopen(fName, "rb");
    if (!fp) {
        perror("CANNOT open file");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_SET);
    for (unsigned int i = 0; i < 15; i++) {
        MOD_STATE.mfr_name[i] = (char) fgetc(fp);
    }
    MOD_STATE.mfr_name[15] = '\0';
    fseek(fp, 16, SEEK_SET);
    for (unsigned int i = 0; i < 15; i++) {
        MOD_STATE.mfr_pn[i] = (char) fgetc(fp);
    }
    MOD_STATE.mfr_pn[15] = '\0';
    fseek(fp, 32, SEEK_SET);
    for (unsigned int i = 0; i < 15; i++) {
        MOD_STATE.mfr_sn[i] = (char) fgetc(fp);
    }
    MOD_STATE.mfr_sn[15] = '\0';
    fclose(fp);
}
#endif

#if defined(__AVR__)
void ag_reset(void) {
    printf("reset\n");
    wdt_enable(WDTO_15MS);
}
#elif defined(__XC16__)
void ag_reset(void) {

}
#endif

void ag_init(void) {
#if MOD_HAS_CLK
    MOD_STATE.caps |= AG_CAP_CLK;
#endif
#if MOD_HAS_1PPS
    MOD_STATE.caps |= AG_CAP_1PPS;
#endif
#if MOD_HAS_JTAG
    MOD_STATE.caps |= AG_CAP_JTAG;
#endif
#if MOD_HAS_USB
    MOD_STATE.caps |= AG_CAP_USB;
#endif
#if MOD_HAS_PCIE
    MOD_STATE.caps |= AG_CAP_PCIE;
#endif

    MOD_STATE.flags = AG_FLAG_EEPROM;
    printf("%s caps: 0x%02X, flags: 0x%02X\n", PREFIX_MC,
           MOD_STATE.caps, MOD_STATE.flags);

    MOD_STATE.addr_d = p_gpio_addr_d();
    p_restore_state();
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
