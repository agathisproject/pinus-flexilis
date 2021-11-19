// SPDX-License-Identifier: GPL-3.0-or-later

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#include "sim/top.h"
#include "cli/cli.h"
#include "agathis/base.h"
#include "agathis/comm.h"

static pthread_t pt_id1, pt_id2;

void sigint_handler(int dummy) {
    pthread_cancel(pt_id1);
}

void *thread_cli (void *vargp) {
    uint8_t parseSts = 1;
    char prompt[12];

    if (MOD_STATE.caps_ext & AG_CAP_EXT_TMC) {
        snprintf(prompt, 12, "-%s@%02d-> ", "TMC", MOD_STATE.addr_d);
    } else {
        snprintf(prompt, 12, "-%s@%02d-> ", "MC", MOD_STATE.addr_d);
    }
    CLI_setPrompt(prompt);

    while (1) {
        printf("%s", CLI_getPrompt());
        CLI_getCmd();
        parseSts = CLI_parseCmd();
        if (parseSts == 0) {
            CLI_execute();
        }

    }
}

void *thread_comm (void *vargp) {
    while (1) {
        ag_communicate();
    }
}

int main(int argc, char *argv[]) {
    sim_init(argc, argv);
    ag_init();
    CLI_init();

    signal(SIGINT, sigint_handler);

    pthread_create(&pt_id1, NULL, thread_cli, NULL);
    pthread_create(&pt_id2, NULL, thread_comm, NULL);
    pthread_join(pt_id1, NULL);

    sim_finish();
    return EXIT_SUCCESS;
}
