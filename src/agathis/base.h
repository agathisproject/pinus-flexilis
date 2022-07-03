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
    uint8_t caps_hw_ext;    /**< HW capabilities that should be advertised */
    uint8_t caps_hw_int;    /**< HW capabilities that should NOT be advertised */
    uint8_t caps_sw;        /**< SW capabilities set by user */
    uint8_t last_err;
    uint16_t type;
    char mfr_name[16];
    char mfr_pn[16];
    char mfr_sn[16];
    float i5_nom;
    float i5_cutoff;
    float i3_nom;
    float i3_cutoff;
} AG_MC_STATE_t;

extern AG_MC_STATE_t MOD_STATE;

/**
 * @brief info about the other MCs (Management Controllers)
 */
typedef struct {
    uint8_t mac[6];
    uint8_t caps;
    uint8_t last_err;
} AG_RMT_MC_STATE_t;

#define MC_MAX_CNT 16 /** max number of MCs in the chain, including the local one */

typedef struct {
    AG_RMT_MC_STATE_t *state;
    AG_RMT_MC_STATE_t *next;
} AG_RMT_MC_NODE_t;

extern AG_RMT_MC_NODE_t *REMOTE_MODS;

#if defined(__XC16__)
extern SemaphoreHandle_t xSemaphore_MMC;
#endif

void ag_reset(void);

void ag_init(void);

void ag_get_MAC(uint8_t *mac);

void ag_get_MAC_compact(uint32_t *mac);

float ag_get_I5_NOM(void);

float ag_get_I3_NOM(void);

#endif /* AGATHIS_6PLS6RVRFVYEP7NX */
