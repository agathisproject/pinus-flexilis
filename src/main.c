// SPDX-License-Identifier: GPL-3.0-or-later

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "tasks.h"
#include "agathis/base.h"
#include "cli/cli.h"
#include "hw/platform.h"
#include "hw/platform_sim/top.h"
#include "opt/utils.h"

static pthread_t pt_id1, pt_id2;

void sigint_handler(int dummy) {
    pthread_cancel(pt_id1);
    //pthread_cancel(pt_id2);
}

int main(int argc, char *argv[]) {
    sim_Init(argc, argv);
    platform_Init();
    ag_Init();

    signal(SIGINT, sigint_handler);

    pthread_create(&pt_id1, NULL, task_cli, NULL);
    pthread_create(&pt_id2, NULL, task_rf, NULL);
    pthread_join(pt_id1, NULL);
    //pthread_join(pt_id2, NULL);

    ag_Exit();
    sim_Exit();
    return EXIT_SUCCESS;
}
