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
#include <stdlib.h>
#include "../sim/state.h"
#include "../sim/misc.h"
#endif
#include "config.h"

AG_MC_STATE_t MOD_STATE = {0, 0, 0,
                           0, 0, 0, 0xFFFF,
                           "", "", "",
                           0.1f, 0.12f, 1.0f, 1.5f,
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
uint8_t p_get_eeprom_byte(uint16_t addr) {
    return 0xFF;
}
#elif defined(__linux__) || defined(__MINGW64__)
uint8_t p_get_eeprom_byte(uint16_t addr) {
    const char *fName = SIM_STATE.eeprom_path;
    FILE *fp;

    fp = fopen(fName, "rb");
    if (!fp) {
        perror("CANNOT open file");
        exit(EXIT_FAILURE);
    }

    fseek(fp, addr, SEEK_SET);
    uint8_t tmp = (uint8_t) fgetc(fp);
    fclose(fp);

    return tmp;
}
#endif

#if defined(__AVR__) || defined(__XC16__)
void p_get_eeprom_str(uint16_t addr, uint8_t len, char *str) {

}
#elif defined(__linux__) || defined(__MINGW64__)
void p_get_eeprom_str(uint16_t addr, uint8_t len, char *str) {
    const char *fName = SIM_STATE.eeprom_path;
    FILE *fp;

    fp = fopen(fName, "rb");
    if (!fp) {
        perror("CANNOT open file");
        exit(EXIT_FAILURE);
    }

    fseek(fp, addr, SEEK_SET);
    for (unsigned int i = 0; i < len; i++) {
        str[i] = (char) fgetc(fp);
    }
    str[len] = '\0';

    fclose(fp);
}
#endif

/**
 * read floating point value from EEPROM
 *
 * Reads a 16b value from EEPROM as a integer, then divide by 100
 * Possible values are between -327.68 and 327.67
 * @param addr
 * @param f
 * @return
 */
#if defined(__AVR__) || defined(__XC16__)
float p_get_eeprom_float(uint16_t addr) {
    return 0.0;
}
#elif defined(__linux__) || defined(__MINGW64__)
float p_get_eeprom_float(uint16_t addr) {
    const char *fName = SIM_STATE.eeprom_path;
    FILE *fp;

    fp = fopen(fName, "rb");
    if (!fp) {
        perror("CANNOT open file");
        exit(EXIT_FAILURE);
    }

    int16_t tmp = 0;
    fseek(fp, addr, SEEK_SET);
    tmp = (int16_t) fgetc(fp);
    fseek(fp, (addr + 1), SEEK_SET);
    tmp += (int16_t) (fgetc(fp) << 8);

    fclose(fp);
    return ((float) tmp / 100.0f);
}
#endif

void p_restore_state(void) {
    uint8_t ver = p_get_eeprom_byte(0);
    printf("%s EEPROM ver %d\n", PREFIX_MC, ver);
    if (ver == 0xFF) {
        return;
    }

    p_get_eeprom_str(16, 16, MOD_STATE.mfr_name);
    p_get_eeprom_str(32, 16, MOD_STATE.mfr_pn);
    p_get_eeprom_str(48, 16, MOD_STATE.mfr_sn);

    MOD_STATE.i5_nom = p_get_eeprom_float(64);
    MOD_STATE.i5_cutoff = p_get_eeprom_float(66);
    MOD_STATE.i3_nom = p_get_eeprom_float(68);
    MOD_STATE.i3_cutoff = p_get_eeprom_float(70);
}

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
#if MOD_HAS_EEPROM
    MOD_STATE.caps_int = AG_CAP_INT_EEPROM;
#endif

#if MOD_HAS_PWR
    MOD_STATE.caps_ext |= AG_CAP_EXT_PWR;
#endif
#if MOD_HAS_CLK
    MOD_STATE.caps_ext |= AG_CAP_EXT_CLK;
#endif
#if MOD_HAS_1PPS
    MOD_STATE.caps_ext |= AG_CAP_EXT_1PPS;
#endif
#if MOD_HAS_JTAG
    MOD_STATE.caps_ext |= AG_CAP_EXT_JTAG;
#endif
#if MOD_HAS_USB
    MOD_STATE.caps_ext |= AG_CAP_EXT_USB;
#endif
#if MOD_HAS_PCIE
    MOD_STATE.caps_ext |= AG_CAP_EXT_PCIE;
#endif

    MOD_STATE.addr_d = p_gpio_addr_d();
    MOD_STATE.addr_i2c = (I2C_OFFSET + MOD_STATE.addr_d);
    if (MOD_STATE.addr_d == 0) {
        MOD_STATE.caps_ext |= AG_CAP_EXT_TMC;
        MOD_STATE.addr_u = (MOD_STATE.addr_d + 1);
        //MOD_STATE.addr_i2c = (I2C_OFFSET + 1); // TODO: for test, remove on release
    } else if (MOD_STATE.addr_d == (MC_MAX_CNT - 1)) {
        MOD_STATE.caps_ext |= AG_CAP_EXT_TMC;
        MOD_STATE.addr_u = 1;
    } else {
        MOD_STATE.addr_u = (MOD_STATE.addr_d + 1);
    }
    p_gpio_addr_u(MOD_STATE.addr_u);

    printf("%s caps: 0x%02X / 0x%02X\n", PREFIX_MC,
           MOD_STATE.caps_ext, MOD_STATE.caps_int);
    printf("%s addr: 0x%02X / 0x%02X (0x%02X)\n", PREFIX_MC,
           MOD_STATE.addr_d, MOD_STATE.addr_u, MOD_STATE.addr_i2c);

#if MOD_HAS_EEPROM
    p_restore_state();
#endif
    MOD_STATE.last_err = 0;
}

#if defined(__AVR__)
float ag_get_I5_NOM(void) {
    return 0.0f;
}
#elif defined(__XC16__)
float ag_get_I5_NOM(void) {
    return 0.0f;
}
#else
float ag_get_I5_NOM(void) {
    return getValue_random((MOD_STATE.i5_nom * 0.7f), 5);
}
#endif

#if defined(__AVR__)
float ag_get_I3_NOM(void) {
    return 0.0f;
}
#elif defined(__XC16__)
float ag_get_I3_NOM(void) {
    return 0.0f;
}
#else
float ag_get_I3_NOM(void) {
    return getValue_random((MOD_STATE.i3_nom * 0.5f), 5);
}
#endif
