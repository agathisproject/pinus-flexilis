// SPDX-License-Identifier: GPL-3.0-or-later

#include "../platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "state.h"

static char p_log_path[SIM_PATH_LEN];

void pltf_Init(void) {
    struct stat st;
    if ((stat("log", &st) != 0) || !(S_ISDIR(st.st_mode))) {
        if (mkdir("log", S_IRWXU | S_IRWXG) != 0) {
            printf("CANNOT CREATE log folder\n");
            exit(EXIT_FAILURE);
        };
    }

    FILE *fp;

    sprintf(p_log_path, "./log/last_run_%03d.log", SIM_STATE.id);
    fp = fopen(p_log_path, "w");
    if (!fp) {
        perror("CANNOT create file");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    printf("log file: '%s'\n", p_log_path);
}

void pltf_ShowHW(void) {
    printf("platform: simulator\n");
}

void pltf_ShowSW(void) {
    printf("APP_NAME: %s\n", APP_NAME);
    printf("APP_VER: %d\n", 0);
}

const char * pltf_GetParamStr(PltfParamId_t param_id) {
    switch (param_id) {
        case PLTF_LOG_PATH: {
            return p_log_path;
        }
        default: {
            return NULL;
        }
    }
}

void pltf_GetID(uint8_t *mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = SIM_STATE.mac[i];
    }
}

void pltf_GetIDCompact(uint32_t *mac) {
    // *INDENT-OFF*
    mac[1] = ((uint32_t) SIM_STATE.mac[5] << 16) | ((uint32_t) SIM_STATE.mac[4] << 8) | SIM_STATE.mac[3];
    mac[0] = ((uint32_t) SIM_STATE.mac[2] << 16) | ((uint32_t) SIM_STATE.mac[1] << 8) | SIM_STATE.mac[0];
    // *INDENT-ON*
}

void pltf_Reset(void) {
    printf("RESET !!!\n");
}

void pltf_PwrOn(void) {
    printf("board POWER ON\n");
}

void pltf_PwrOff(void) {
    printf("board POWER OFF\n");
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
