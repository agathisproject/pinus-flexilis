// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdlib.h>

#include "cli/cli.h"

int main(void) {
    uint8_t parseSts = 1;

    printf("Agathis CLI simulator\n");

    while (1) {
        printf("%s", CLI_PROMPT);
        CLI_Get_Cmd();
        parseSts = CLI_Parse_Cmd();
        if (parseSts == 0) {
            CLI_Execute();
        }
    }
    return EXIT_SUCCESS;
}
