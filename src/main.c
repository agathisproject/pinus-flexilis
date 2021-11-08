// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sys/stat.h"

#include "cli/cli.h"
#include "agathis/base.h"
#include "sim/state.h"

void p_show_help(const char *appName) {
    printf("Usage: pinus-flexilis [OPTIONS]\n");
    printf("\n");
    printf("  -e <EEPROM_FILE_PATH>\n");
    printf("  -c <CONFIG_FILE_PATH>\n");
    exit(EXIT_SUCCESS);
}

void p_test_eeprom(void) {
    const char *f_path = SIM_STATE.eeprom_path;
    FILE *fp;
    struct stat s;

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
    FILE *fp;
    struct stat s;

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

void p_load_config(void) {

}

#define FILE_LEN 64

void sim_init(int argc, char *argv[]) {
    SIM_STATE.eeprom_path = (char *) malloc(FILE_LEN * sizeof(char));
    if (SIM_STATE.eeprom_path == NULL) {
        printf("CANNOT allocate memory: %s\n", __func__);
        exit(EXIT_FAILURE);
    }
    strncpy(SIM_STATE.eeprom_path, "EEPROM.BIN", FILE_LEN);

    SIM_STATE.config_path = (char *) malloc(FILE_LEN * sizeof(char));
    if (SIM_STATE.config_path == NULL) {
        printf("CANNOT allocate memory: %s\n", __func__);
        exit(EXIT_FAILURE);
    }
    strncpy(SIM_STATE.config_path, "config.ini", FILE_LEN);

    int opt = 0;

    while ((opt = getopt (argc, argv, "he:c:")) != -1) {
        switch (opt) {
            case 'h':
                p_show_help(argv[0]);
                break;
            case 'e':
                if (strlen(optarg) <= (FILE_LEN - 1)) {
                    if (strstr(optarg, ".BIN") != NULL) {
                        strncpy(SIM_STATE.eeprom_path, optarg, FILE_LEN);
                    } else {
                        printf("INCORRECT file name: '%s', using default\n", optarg);
                        strncpy(SIM_STATE.eeprom_path, "EEPROM.BIN", FILE_LEN);
                    }
                } else {
                    printf("INCORRECT file name: '%s', using default\n", optarg);
                    strncpy(SIM_STATE.eeprom_path, "EEPROM.BIN", FILE_LEN);
                }
                break;
            case 'c':
                if (strlen(optarg) <= (FILE_LEN - 1)) {
                    if (strstr(optarg, ".ini") != NULL) {
                        strncpy(SIM_STATE.config_path, optarg, FILE_LEN);
                    } else {
                        printf("INCORRECT file name: '%s', using default\n", optarg);
                        strncpy(SIM_STATE.config_path, "config.ini", FILE_LEN);
                    }
                } else {
                    printf("INCORRECT file name: '%s', using default\n", optarg);
                    strncpy(SIM_STATE.config_path, "config.ini", FILE_LEN);
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

    srand((unsigned int) time(NULL));
    printf("Agathis CLI simulator\n");
}

int main(int argc, char *argv[]) {
    sim_init(argc, argv);

    ag_init();
    CLI_init();

    uint8_t parseSts = 1;
    while (1) {
        printf("%s", CLI_getPrompt());
        CLI_getCmd();
        parseSts = CLI_parseCmd();
        if (parseSts == 0) {
            CLI_execute();
        }
    }
    return EXIT_SUCCESS;
}
