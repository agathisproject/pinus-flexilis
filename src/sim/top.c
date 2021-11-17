// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sys/stat.h"

#include "../opt/ini/ini.h"
#include "state.h"
#include "top.h"

void p_show_help(const char *appName) {
    printf("Usage: pinus-flexilis [OPTIONS]\n");
    printf("\n");
    printf("  -e <EEPROM_FILE_PATH>\n");
    printf("  -c <CONFIG_FILE_PATH>\n");
    exit(EXIT_SUCCESS);
}

void p_test_eeprom(void) {
    const char *f_path = SIM_STATE.eeprom_path;
    struct stat s;
    FILE *fp;

    if (stat(f_path, &s) == -1) {
        fp = fopen(f_path, "wb");
        if (!fp) {
            perror("CANNOT create file");
            exit(EXIT_FAILURE);
        }
        for (unsigned int i = 0; i < 1024; i ++) {
            fputc(0xFF, fp);
        }
        fclose(fp);
        printf("created new '%s'\n", f_path);
    }
}

void p_test_config(void) {
    const char *f_path = SIM_STATE.config_path;
    struct stat s;
    FILE *fp;

    if (stat(f_path, &s) == -1) {
        fp = fopen(f_path, "wb");
        if (!fp) {
            perror("CANNOT create file");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "[default]\n");
        fprintf(fp, "addr_d = 0x1\n");
        fclose(fp);
        printf("created new '%s'\n", f_path);
    }
}

typedef struct {
    uint8_t addr_d;
} config_ini_t;

static int p_ini_handler(void* user, const char* section, const char* name,
                         const char* value) {
    config_ini_t *pconfig = (config_ini_t *)user;

    if ((strcmp(section, "default") == 0) && (strcmp(name, "addr_d") == 0)) {
        pconfig->addr_d = (uint8_t) strtol(value, NULL, 16);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void p_load_config(void) {
    config_ini_t config;

    if (ini_parse(SIM_STATE.config_path, p_ini_handler, &config) < 0) {
        printf("CANNOT PARSE '%s'\n", SIM_STATE.config_path);
    }

    SIM_STATE.addr_d = config.addr_d;
}

void p_mkfifo(void) {
    char f_path[32] = "";
    struct stat s;

    if (SIM_STATE.addr_d == 0) {
        return;
    }

    if (stat("run", &s) == -1) {
        if (mkdir("run", 0755) != 0) {
            perror("CANNOT create 'run' folder");
            exit(EXIT_FAILURE);
        }
    }

    snprintf(f_path, 32, "run/slot_%d", SIM_STATE.addr_d);
    if (stat(f_path, &s) == -1) {
        if (mkfifo(f_path, 0755) != 0) {
            perror("CANNOT create pipe");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("pipe already exists\n");
    }
}

void sim_init(int argc, char *argv[]) {
    strncpy(SIM_STATE.eeprom_path, "EEPROM.BIN", SIM_PATH_LEN);
    strncpy(SIM_STATE.config_path, "config.ini", SIM_PATH_LEN);

    int opt = 0;

    while ((opt = getopt (argc, argv, "he:c:")) != -1) {
        switch (opt) {
            case 'h':
                p_show_help(argv[0]);
                break;
            case 'e':
                if (strlen(optarg) <= (SIM_PATH_LEN - 1)) {
                    if (strstr(optarg, ".BIN") != NULL) {
                        strncpy(SIM_STATE.eeprom_path, optarg, SIM_PATH_LEN);
                    } else {
                        printf("INCORRECT file name: '%s', using default\n", optarg);
                        strncpy(SIM_STATE.eeprom_path, "EEPROM.BIN", SIM_PATH_LEN);
                    }
                } else {
                    printf("INCORRECT file name: '%s', using default\n", optarg);
                    strncpy(SIM_STATE.eeprom_path, "EEPROM.BIN", SIM_PATH_LEN);
                }
                break;
            case 'c':
                if (strlen(optarg) <= (SIM_PATH_LEN - 1)) {
                    if (strstr(optarg, ".ini") != NULL) {
                        strncpy(SIM_STATE.config_path, optarg, SIM_PATH_LEN);
                    } else {
                        printf("INCORRECT file name: '%s', using default\n", optarg);
                        strncpy(SIM_STATE.config_path, "config.ini", SIM_PATH_LEN);
                    }
                } else {
                    printf("INCORRECT file name: '%s', using default\n", optarg);
                    strncpy(SIM_STATE.config_path, "config.ini", SIM_PATH_LEN);
                }
                break;
            default:
                break;
        }
    }
    printf("EEPROM file: '%s'\n", SIM_STATE.eeprom_path);
    printf("config file: '%s'\n", SIM_STATE.config_path);
    p_test_eeprom();
    p_test_config();
    p_load_config();
    p_mkfifo();

    srand((unsigned int) time(NULL));
    printf("Agathis CLI simulator\n");
}

void p_rmfifo(void ) {
    char f_path[32] = "";

    if (SIM_STATE.addr_d == 0) {
        return;
    }

    snprintf(f_path, 32, "run/slot_%d", SIM_STATE.addr_d);
    if (remove(f_path) != 0) {
        perror("CANNOT remove pipe");
    }
}

void sim_finish(void) {
    printf("\n");
    p_rmfifo();
}
