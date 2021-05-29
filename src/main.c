// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdlib.h>

#include "cli/cli.h"
#include "agathis/base.h"

int main(void) {
    uint8_t parseSts = 1;

    printf("Agathis CLI simulator\n");
    ag_init();
    CLI_init();

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
