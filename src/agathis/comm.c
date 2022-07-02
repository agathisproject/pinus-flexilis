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
#elif defined(__linux__)
#include <dirent.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../sim/state.h"
#endif
#include "config.h"
#include "base.h"

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
    void *buf;
    ssize_t nb_rx;

    /* Determine max. msg size; allocate buffer to receive msg */
    if (mq_getattr(mq_des, &attr) == -1) {
        perror("CANNOT get mq attr");
        return;
    }
    buf = malloc((size_t) attr.mq_msgsize);
    if (buf == NULL) {
        printf("CANNOT allocate RX buffer\n");
        return;
    }

    nb_rx = mq_receive(mq_des, buf, (size_t) attr.mq_msgsize, NULL);
    if (nb_rx == -1) {
        perror("RX failure");
        free(buf);
        return;
    }
    printf("DBG RX@%d %zd bytes\n", SIM_STATE.id, nb_rx);
    free(buf);
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

int ag_tx(int mc_id, uint8_t *buff, uint8_t nb) {
#if defined(__linux__)
    char mq_name[32] = "";
    char dst_name[32] = "";

    snprintf(mq_name, 32, "%s%03d", SIM_MQ_PREFIX, SIM_STATE.id);

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
        if (strncmp(mq_name, dir->d_name, 32) == 0) {
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
        printf("TX to %s\n", dst_name);
        if (mq_send (queue, "hello", 6, 0) == -1) {
            perror ("CANNOT send msg");
            continue;
        }

        mq_close(queue);
    }
    closedir(d);
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
    if ((ts_now % 5) == 0) {
        ag_tx(-1, (uint8_t *) "hello", 5);
    }

    sleep(1);
#endif
}
