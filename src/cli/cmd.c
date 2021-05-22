// SPDX-License-Identifier: GPL-3.0-or-later

#include "cli_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CliCmdReturn_t p_env(ParsedCmd_t *cmdp) {
    if (cmdp->nParams == 0) {
        printf("CLI\n");
        return CMD_DONE;
    } else if (cmdp->nParams == 3) {
        return CMD_DONE;
    } else {
        return CMD_WRONG_N;
    }
}

CliCmdReturn_t info(ParsedCmd_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("MFR_NAME\n");
    printf("MFR_PN\n");
    printf("MFR_SN\n");
    return CMD_DONE;
}

CliCmdReturn_t dev(ParsedCmd_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("gpio\n");
    return CMD_DONE;
}

CliCmdReturn_t mod(ParsedCmd_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("mod1\n");
    printf("mod2\n");
    printf("mod3\n");
    return CMD_DONE;
}

CliCmdReturn_t gpio(ParsedCmd_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}

#define CMD_CNT 4

unsigned int Get_Cmd_Cnt(void) {
    return CMD_CNT;
}

static CliCmd_t _CMDS_ARRAY[CMD_CNT] = {
    {"?", "", "show module info", 0, 0, &info},
    {"dev", "", "show devices", 0, 1, &dev},
    {"mod", "", "show modules", 0, 0, &mod},
    {"gpio", "", "show modules", 1, 1, &gpio},
};

CliCmd_t *CMDS = _CMDS_ARRAY;
