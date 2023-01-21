// SPDX-License-Identifier: GPL-3.0-or-later

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "tasks.h"
#include "agathis/base.h"
#include "cli/cli.h"
#include "hw/platform_sim/top.h"

static pthread_t pt_id1, pt_id2;

void sigint_handler(int dummy) {
    pthread_cancel(pt_id1);
    //pthread_cancel(pt_id2);
}

int main(int argc, char *argv[]) {
    sim_init(argc, argv);
    ag_init();

    signal(SIGINT, sigint_handler);

    pthread_create(&pt_id1, NULL, task_cli, NULL);
    pthread_create(&pt_id2, NULL, task_rf, NULL);
    pthread_join(pt_id1, NULL);
    //pthread_join(pt_id2, NULL);

    sim_finish();
    return EXIT_SUCCESS;
}
