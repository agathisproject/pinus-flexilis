// SPDX-License-Identifier: GPL-3.0-or-later

#include <dirent.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../../opt/inih/ini.h"
#include "state.h"
#include "top.h"

static void p_show_help(const char *appName) {
    printf("Usage: pinus-flexilis [OPTIONS]\n");
    printf("\n");
    printf("  -e <EEPROM_FILE_PATH>\n");
    printf("  -c <CONFIG_FILE_PATH>\n");
    printf("  -b run with no console\n");
    exit(EXIT_SUCCESS);
}

static void p_test_eeprom(void) {
    const char *f_path = SIM_STATE.eeprom_path;
    FILE *fp;

    if (access(f_path, (R_OK | W_OK)) != 0) {
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
    printf("EEPROM file: '%s'\n", SIM_STATE.eeprom_path);
}

static void p_test_config(void) {
    const char *f_path = SIM_STATE.config_path;
    FILE *fp;

    if (access(f_path, (R_OK | W_OK)) != 0) {
        fp = fopen(f_path, "wb");
        if (!fp) {
            perror("CANNOT create file");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "[default]\n");
        fprintf(fp, "id = %d\n", SIM_STATE.id);
        fclose(fp);
        printf("created new '%s'\n", f_path);
    }
    printf("config file: '%s'\n", SIM_STATE.config_path);
}

//typedef struct {
//    uint8_t addr_d;
//} config_ini_t;

//static int p_ini_handler(void* user, const char* section, const char* name,
//                         const char* value) {
//    config_ini_t *pconfig = (config_ini_t *)user;
//
//    if ((strcmp(section, "default") == 0) && (strcmp(name, "addr_d") == 0)) {
//        pconfig->addr_d = (uint8_t) strtol(value, NULL, 16);
//    } else {
//        return 0;  /* unknown section/name, error */
//    }
//    return 1;
//}

static void p_load_config(void) {
//    config_ini_t config;
//
//    if (ini_parse(SIM_STATE.config_path, p_ini_handler, &config) < 0) {
//        printf("CANNOT PARSE '%s'\n", SIM_STATE.config_path);
//    }
}

static void p_guess_id(void) {
    if (access("/dev/mqueue", (R_OK | W_OK)) != 0) {
        perror("CANNOT find mqueue support");
        exit(EXIT_FAILURE);
    }

    DIR *d = opendir("/dev/mqueue");
    if (d == NULL) {
        printf("%s\n", "CANNOT open folder");
        exit(EXIT_FAILURE);
    }

    long max = -1;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        size_t tmp_len = strlen(SIM_MQ_PREFIX);
        if (strncmp(SIM_MQ_PREFIX, dir->d_name, tmp_len) != 0) {
            continue;
        }
        //printf("DBG %s\n", dir->d_name);
        long tmp = strtol((dir->d_name + tmp_len), NULL, 10);
        if (tmp > max) {
            max = tmp;
        }
    }
    closedir(d);

    if (max < 0) {
        SIM_STATE.id = 0;
    } else if ((max >= 0) && (max < 255)) {
        SIM_STATE.id = (uint8_t) (max + 1);
    } else {
        printf("%s - line %d\n", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }
    //printf("DBG sim id = %d\n", SIM_STATE.id);
}

void p_start_IPC(void) {
    char mq_name[32] = "";

    snprintf(mq_name, 32, "/%s%03d", SIM_MQ_PREFIX, SIM_STATE.id);
    SIM_STATE.msg_queue = mq_open(mq_name, (O_RDONLY | O_CREAT), 0660, NULL);
    if (SIM_STATE.msg_queue == -1) {
        perror("CANNOT create mq");
        exit(EXIT_FAILURE);
    }
    printf("start IPC\n");
}

void sim_init(int argc, char *argv[]) {
    int opt = 0;

    while ((opt = getopt (argc, argv, "he:c:b")) != -1) {
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
                        strncpy(SIM_STATE.eeprom_path, "cfg/EEPROM.BIN", SIM_PATH_LEN);
                    }
                } else {
                    printf("INCORRECT file name: '%s', using default\n", optarg);
                    strncpy(SIM_STATE.eeprom_path, "cfg/EEPROM.BIN", SIM_PATH_LEN);
                }
                break;
            case 'c':
                if (strlen(optarg) <= (SIM_PATH_LEN - 1)) {
                    if (strstr(optarg, ".ini") != NULL) {
                        strncpy(SIM_STATE.config_path, optarg, SIM_PATH_LEN);
                    } else {
                        printf("INCORRECT file name: '%s', using default\n", optarg);
                        strncpy(SIM_STATE.config_path, "cfg/config.ini", SIM_PATH_LEN);
                    }
                } else {
                    printf("INCORRECT file name: '%s', using default\n", optarg);
                    strncpy(SIM_STATE.config_path, "cfg/config.ini", SIM_PATH_LEN);
                }
                break;
            case 'b':
                SIM_STATE.sim_flags |= SIM_FLAG_NO_CONSOLE;
                break;
            default:
                break;
        }
    }

    p_guess_id();
    SIM_STATE.mac[0] = SIM_STATE.id;

    if (access("cfg", (R_OK | W_OK | X_OK)) != 0) {
        if (mkdir("cfg", 0775) == -1) {
            perror("CANNOT create folder");
            exit(EXIT_FAILURE);
        }
    }

    char f_path[SIM_PATH_LEN] = "";

    snprintf(f_path, SIM_PATH_LEN, "cfg/EEPROM_%03d.BIN", SIM_STATE.id);
    strncpy(SIM_STATE.eeprom_path, f_path, SIM_PATH_LEN);
    snprintf(f_path, SIM_PATH_LEN, "cfg/config_%03d.ini", SIM_STATE.id);
    strncpy(SIM_STATE.config_path, f_path, SIM_PATH_LEN);

    p_test_eeprom();
    p_test_config();
    p_load_config();

    p_start_IPC();

    //printf("DBG sim flags %x\n", SIM_STATE.sim_flags);
    if ((SIM_STATE.sim_flags & SIM_FLAG_NO_CONSOLE) != 0) {
        printf("running with no console\n");
    }
    srand((unsigned int) time(NULL));
    printf("Agathis CLI simulator\n");
}

void p_stop_IPC(void ) {
    char mq_name[32] = "";

    snprintf(mq_name, 32, "/%s%03d", SIM_MQ_PREFIX, SIM_STATE.id);
    if (mq_close(SIM_STATE.msg_queue) == -1) {
        perror("CANNOT close mq");
        exit(EXIT_FAILURE);
    }
    if (mq_unlink(mq_name) == -1) {
        perror("CANNOT unlink mq");
        exit(EXIT_FAILURE);
    }
    printf("stop IPC\n");
}

void sim_finish(void) {
    printf("\n");
    p_stop_IPC();
}
