// SPDX-License-Identifier: GPL-3.0-or-later

#include "comm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__AVR__)
#include <avr/wdt.h>
#elif defined(__linux__)
#include <dirent.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>

#include "../sim/state.h"
#endif

#include "defs.h"
#include "base.h"
#include "../hw/gpio.h"
#include "../hw/storage.h"
#include "../platform/platform.h"

#if defined(__AVR__)
void ag_rx_cback(void) {

}
#elif defined(__XC16__)
void ag_rx_cback(void) {

}
#elif defined(__linux__)
static void p_mq_notify(void);

static void ag_rx_cback(union sigval sv) {
    mqd_t mq_des = *((mqd_t *) sv.sival_ptr);
    struct mq_attr attr;

    /* Determine max. msg size; allocate buffer to receive msg */
    if (mq_getattr(mq_des, &attr) == -1) {
        perror("CANNOT get mq attr");
        return;
    }

    uint8_t *buff;

    buff = (uint8_t *) malloc((size_t) attr.mq_msgsize);
    if (buff == NULL) {
        printf("CANNOT allocate RX buffer\n");
        return;
    }

    ssize_t nb_rx = mq_receive(mq_des, (char *) buff, (size_t) attr.mq_msgsize,
                               NULL);
    if (nb_rx == -1) {
        perror("RX failure");
        free(buff);
        return;
    }
    //printf("DBG RX@%d %zd bytes\n", SIM_STATE.id, nb_rx);
    //printf("DBG RX@%d dst: %02x:%02x:%02x:%02x:%02x:%02x\n", SIM_STATE.id, buff[5], buff[4], buff[3], buff[2], buff[1], buff[0]);
    //printf("DBG RX@%d src: %02x:%02x:%02x:%02x:%02x:%02x\n", SIM_STATE.id, buff[11], buff[10], buff[9], buff[8], buff[7], buff[6]);
    if (nb_rx == AG_MSG_LEN) {
        uint32_t mac_dst[2];
        uint32_t mac_src[2];
        uint32_t mac_lcl[2];
        uint8_t mac[6];

        stor_get_MAC(mac);
        mac_dst[1] = ((uint32_t) buff[5] << 16) | ((uint32_t) buff[4] << 8) | buff[3];
        mac_dst[0] = ((uint32_t) buff[2] << 16) | ((uint32_t) buff[1] << 8) | buff[0];
        mac_src[1] = ((uint32_t) buff[11] << 16) | ((uint32_t) buff[10] << 8) | buff[9];
        mac_src[0] = ((uint32_t) buff[8] << 16) | ((uint32_t) buff[7] << 8) | buff[6];
        mac_lcl[1] = ((uint32_t) mac[5] << 16) | ((uint32_t) mac[4] << 8) | mac[3];
        mac_lcl[0] = ((uint32_t) mac[2] << 16) | ((uint32_t) mac[1] << 8) | mac[0];
        if ((mac_dst[1] == 0x00FFFFFF) && (mac_dst[0] == 0x00FFFFFF)) {
            //printf("DBG RX@%d brcst from %06x:%06x\n", SIM_STATE.id, mac_src[1], mac_src[0]);
            if ((buff[12] == AG_PROTO_VER1) && (buff[13] == AG_PKT_TYPE_STATUS)) {
                ag_add_remote_mod(mac_src, buff[12 + 4]);
            }
        }
        if ((mac_dst[1] == mac_lcl[1]) && (mac_dst[0] == mac_lcl[0])) {
            //printf("DBG RX@%d msg from %06x:%06x\n", SIM_STATE.id, mac_src[1], mac_src[0]);
            if ((buff[12] == AG_PROTO_VER1) && (buff[13] == AG_PKT_TYPE_CMD)) {
                if (buff[14] == AG_CMD_ID) {
                    ag_id_external();
                } else if (buff[14] == AG_CMD_RESET) {
                    ag_reset();
                }
            }
        }
    } else {
        printf("INCORRECT number of bytes RX\n");
    }
    free(buff);
    p_mq_notify();
}

static void p_mq_notify(void) {
    struct sigevent sev;

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = ag_rx_cback;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &(SIM_STATE.msg_queue);
    if (mq_notify(SIM_STATE.msg_queue, &sev) == -1) {
        perror("mq_notify");
        exit(EXIT_FAILURE);
    }
}
#endif

int ag_comm_tx(uint32_t dst_mac1, uint32_t dst_mac0, uint8_t *buff,
               uint8_t nb) {
#if defined(__linux__)
    char mq_name[SIM_PATH_LEN] = "";
    char dst_name[SIM_PATH_LEN] = "";
    uint8_t mac[6];
    char *send_data = (char *) malloc((nb + 12) * sizeof(char));

    if (send_data == NULL) {
        printf("%s\n", "CANNOT malloc");
        exit(EXIT_FAILURE);
    }

    snprintf(mq_name, SIM_PATH_LEN, "%s%03d", SIM_MQ_PREFIX, SIM_STATE.id);

    DIR *d = opendir("/dev/mqueue");
    if (d == NULL) {
        printf("%s\n", "CANNOT open folder");
        exit(EXIT_FAILURE);
    }

    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        size_t tmp_len = strlen(SIM_MQ_PREFIX);
        if (strncmp(SIM_MQ_PREFIX, dir->d_name, tmp_len) != 0) {
            continue;
        }
        if (strncmp(mq_name, dir->d_name, SIM_PATH_LEN) == 0) {
            continue;
        }

        dst_name[0] = '/';
        dst_name[1] = '\0';
        strcat(dst_name, dir->d_name);
        mqd_t queue = mq_open(dst_name, O_WRONLY);
        if (queue == -1) {
            perror("CANNOT create mq");
            continue;
        }

        //printf("DBG TX@%d to %s\n", SIM_STATE.id, dst_name);
        send_data[0] = (char) (dst_mac0 & 0xFF);
        send_data[1] = (char) (dst_mac0 >> 8);
        send_data[2] = (char) (dst_mac0 >> 16);
        send_data[3] = (char) (dst_mac1 & 0xFF);
        send_data[4] = (char) (dst_mac1 >> 8);
        send_data[5] = (char) (dst_mac1 >> 16);
        stor_get_MAC(mac);
        for (int i = 6; i < 12; i++) {
            send_data[i] = (char) mac[i - 6];
        }
        for (int i = 12; i < (nb + 12); i++) {
            send_data[i] = (char) buff[i - 12];
        }
        if (mq_send(queue, (const char *) send_data, nb, 0) == -1) {
            perror("CANNOT send msg");
            continue;
        }

        mq_close(queue);
    }
    closedir(d);
    free(send_data);
    return 0;
#endif
}

void ag_comm_init(void) {
#if defined(__linux)
    p_mq_notify();
#endif
}

void ag_comm_main(void) {
#if defined(__AVR__)

#elif defined(__XC16__)

#elif defined(__linux__)
    time_t ts_now = time(NULL);
    uint8_t buff[AG_MSG_LEN];

    if ((ts_now % 5) == 0) {
        memset(buff, 0, AG_MSG_LEN * sizeof (uint8_t));
        buff[0] = AG_PROTO_VER1;
        buff[1] = AG_PKT_TYPE_STATUS;
        buff[4] = MOD_STATE.caps_sw;
        ag_comm_tx(0x00FFFFFF, 0x00FFFFFF, buff, AG_MSG_LEN);
    }

    sleep(1);
    ag_upd_remote_mods();
    ag_upd_alarm();
#endif
}
