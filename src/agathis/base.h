// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AGATHIS_6PLS6RVRFVYEP7NX
#define AGATHIS_6PLS6RVRFVYEP7NX
/** @file */

#include <stdint.h>
#if defined(__XC16__)
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#endif
#include "cmds.h"

#define PREFIX_MC "[ MC]"
#define I2C_OFFSET 0x20

/**
 * @brief state of the local MC (Management Controller)
 */
typedef struct {
    uint8_t addr_d;        /**< down-trunk management address */
    uint8_t addr_u;        /**< up-trunk management address */
    uint8_t addr_i2c;      /**< MC I2C address */
    uint8_t flags;
    uint8_t caps;
    uint8_t last_err;
    uint16_t type;
    char mfr_name[16];
    char mfr_pn[16];
    char mfr_sn[16];
} AG_MC_STATE_t;

extern AG_MC_STATE_t MOD_STATE;

typedef enum {
    MC_NOT_PRESENT,
    MC_INVALID,
    MC_PRESENT,
} AG_MC_SCAN_STATE;

/**
 * @brief info about the other MCs (Management Controllers)
 */
typedef struct {
    AG_MC_SCAN_STATE state;
    uint8_t pow_rst;
    uint8_t alarms;
} AG_MC_SCAN_INFO_t;

#define MC_MAX_CNT 16 /** max number of MCs in the chain, including the local one */
extern AG_MC_SCAN_INFO_t REMOTE_MODS[MC_MAX_CNT - 1];

#if defined(__XC16__)
extern SemaphoreHandle_t xSemaphore_MMC;
#endif

void ag_reset(void);

void ag_init(void);

#endif /* AGATHIS_6PLS6RVRFVYEP7NX */
