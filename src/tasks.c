// SPDX-License-Identifier: GPL-3.0-or-later

#include "tasks.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "agathis/comm.h"
#include "cli/cli.h"
#include "hw/storage.h"
#include "sim/state.h"

static void p_CLI_init_prompt(void) {
    char prompt[CLI_PROMPT_SIZE];
    uint32_t mac[2];

    stor_get_MAC_compact(mac);
    snprintf(prompt, CLI_PROMPT_SIZE, "[%06x:%06x]$ ", mac[1], mac[0]);
    printf("press ? for help\n");
    CLI_setPrompt(prompt);
}

#if defined(__linux__)
void *task_cli (void *vargp) {
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
#elif defined (CONFIG_IDF_TARGET)
void task_cli(void *pvParameter) {
}
#endif

#if defined(__linux__)
void *task_rf (void *vargp) {
    ag_comm_init();

    while (1) {
        ag_comm_main();
    }
}
#elif defined (CONFIG_IDF_TARGET)
void task_rf(void *pvParameter) {
}
#endif
