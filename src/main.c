// SPDX-License-Identifier: GPL-3.0-or-later

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "sim/state.h"
#include "sim/top.h"
#include "cli/cli.h"
#include "agathis/base.h"
#include "agathis/comm.h"

static pthread_t pt_id1, pt_id2;

void sigint_handler(int dummy) {
    pthread_cancel(pt_id1);
    //pthread_cancel(pt_id2);
}

static void p_CLI_init_prompt(void) {
    char prompt[12];

    snprintf(prompt, 12, "%02x:%02x:%02x-> ", SIM_STATE.mac[2], SIM_STATE.mac[1],
             SIM_STATE.mac[0]);
    printf("press ? for help\n");
    CLI_setPrompt(prompt);
}

void *thread_cli (void *vargp) {
    uint8_t parseSts = 1;

    if ((SIM_STATE.sim_flags & SIM_FLAG_NO_CONSOLE) != 0) {
        while (1) {
            sleep(1);
        }
    } else {
        p_CLI_init_prompt();
        while (1) {
            printf("%s", CLI_getPrompt());
            CLI_getCmd();
            parseSts = CLI_parseCmd();
            if (parseSts == 0) {
                CLI_execute();
            }
        }
    }
}

void *thread_comm (void *vargp) {
    ag_comm_init();

    while (1) {
        ag_comm_main();
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
    //pthread_join(pt_id2, NULL);

    sim_finish();
    return EXIT_SUCCESS;
}
