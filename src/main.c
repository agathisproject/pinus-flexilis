// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "cli/cli.h"
#include "agathis/base.h"
#include "sim/state.h"

void p_parse_addr(char *arg) {
    uint8_t tmp = (uint8_t) strtoul(arg, NULL, 16);
    if (tmp < MC_MAX_CNT) {
        SIM_STATE.addr_d = tmp;
    }
}

void sim_init(int argc, char *argv[]) {
    int opt = 0;

    while ((opt = getopt (argc, argv, "a:")) != -1) {
        switch (opt) {
            case 'a':
                p_parse_addr(optarg);
                break;
            default:
                break;
        }
    }
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
