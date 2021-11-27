// SPDX-License-Identifier: GPL-3.0-or-later

#include "comm.h"

#include <stdio.h>
#if defined(__AVR__)
#include <avr/wdt.h>
#include "../hw/gpio.h"
#include "../platform.h"
#elif defined(__XC16__)
#include "../hw/gpio.h"
#include "../platform.h"
#elif defined(__linux__) || defined(__MINGW64__)
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sys/stat.h"
#include "sys/types.h"
#endif
#include "config.h"
#include "base.h"

#if defined(__AVR__)
void ag_receive(void) {

}
#elif defined(__XC16__)
void ag_receive(void) {

}
#elif defined(__linux__) || defined(__MINGW64__)
void ag_rx_cback(void) {
    char f_path[32] = "";
    struct stat s;
    int fd;
    unsigned int nb;
    uint8_t buff[8];

    snprintf(f_path, 32, "run/slot_%d", MOD_STATE.addr_d);
    if (stat(f_path, &s) == -1) {
        return;
    }
    if ((s.st_mode & S_IFMT) != S_IFIFO) {
        return;
    }

    fd = open(f_path, O_RDONLY);
    if (fd == -1) {
        perror("CANNOT open pipe\n");
        return;
    }
    nb = (unsigned int) read(fd, buff, 8);
    close(fd);
    if (nb != 1) {
        printf("UNEXPECTED RX: %d byte(s)\n", nb);
        return;
    }

    nb = 0;
    switch (buff[0]) {
        case 1: {
            nb = AG_CMD_SUMMARY_NB;
            buff[0] = AG_CMD_SUMMARY_NB;
            buff[1] = MOD_STATE.caps_ext;
            buff[2] = 0x0;
            buff[3] = 0x0;
            break;
        }
        default: {
            printf("UNRECOGNIZED cmd %#04x\n", buff[0]);
            break;
        }
    }

    fd = open(f_path, O_WRONLY);
    if (fd == -1) {
        perror("CANNOT open pipe\n");
        return;
    }
    write(fd, buff, (size_t) nb);
    close(fd);
}
#endif

int ag_smbus_block_rd(uint8_t addr, uint8_t cmd, uint8_t *buff, uint8_t nb) {
    char f_path[32] = "";
    struct stat s;
    int fd;
    unsigned int nb_rcv = 0;

    snprintf(f_path, 32, "run/slot_%d", (addr - I2C_OFFSET));
    if (stat(f_path, &s) == -1) {
        return -1;
    }
    if ((s.st_mode & S_IFMT) != S_IFIFO) {
        return -1;
    }

    fd = open(f_path, O_WRONLY);
    if (fd == -1) {
        return -1;
    }
    write(fd, &cmd, 1);
    close(fd);

    usleep(10 * 1000);

    fd = open(f_path, O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    nb_rcv = (unsigned int) read(fd, buff, nb);
    close(fd);

    //printf("DBG: reply %d bytes(s)\n", nb_rcv);
    //printf("DBG: %x %x %x %x\n", buff[0], buff[1], buff[2], buff[3]);
    usleep(90 * 1000);
    return 0;
}

#if defined(__AVR__)
void ag_cmd_summary(uint8_t mod_id, uint8_t *data) {

}
#elif defined(__XC16__)
void ag_cmd_summary(uint8_t mod_id, uint8_t *data) {

}
#elif defined(__linux__) || defined(__MINGW64__)
void ag_cmd_summary(uint8_t mod_id, uint8_t *data) {
    int sts = -1;
    uint8_t buff[AG_CMD_SUMMARY_NB];

    sts = ag_smbus_block_rd((mod_id + I2C_OFFSET), AG_CMD_SUMMARY, buff,
                            AG_CMD_SUMMARY_NB);
    if (sts < 0) {
        REMOTE_MODS[mod_id].state = MC_NOT_PRESENT;
    } else {
        REMOTE_MODS[mod_id].state = MC_PRESENT;
    }
}
#endif

#if defined(__AVR__)
void ag_communicate(void) {

}
#elif defined(__XC16__)
void ag_communicate(void) {

}
#elif defined(__linux__) || defined(__MINGW64__)
void ag_communicate(void) {
    if (MOD_STATE.caps_ext & AG_CAP_EXT_TMC) {
        for (uint8_t i = 1; i < MC_MAX_CNT; i++) {
            ag_cmd_summary(i, NULL);
        }
    } else {
        ag_rx_cback();
    };
}
#endif
