// SPDX-License-Identifier: GPL-3.0-or-later

#include "cli_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CliCmdReturn_t cli(ParsedCmd_t *cmdp) {
    if (cmdp->nParams == 0) {
        printf("CLI\n");
        return CMD_DONE;
    } else if (cmdp->nParams == 3) {
        return CMD_DONE;
    } else {
        return CMD_WRONG_N;
    }
}

void p_info_SW(void) {
    printf("ver: ?\n");
}

void p_info_HW(void) {
    printf("type: ?\n");
    printf("SN: ?\n");
}

CliCmdReturn_t info(ParsedCmd_t *cmdp) {
    if (cmdp->nParams != 1) {
        return CMD_WRONG_N;
    }

    if (strncmp(cmdp->params[0], "sw", CLI_WORD_SIZE) == 0) {
        p_info_SW();
    } else if (strncmp(cmdp->params[0], "hw", CLI_WORD_SIZE) == 0) {
        p_info_HW();
    } else {
        return CMD_WRONG_PARAM;
    }
    return CMD_DONE;
}

#define CMD_CNT 2

unsigned int Get_Cmd_Cnt(void) {
    return CMD_CNT;
}

static CliCmd_t _CMDS_ARRAY[CMD_CNT] = {
    {".cli", "[set key value]", "show/set CLI params", &cli},
    {"info", "[sw|hw]", "show HW/SW info", &info},
};

CliCmd_t *CMDS = _CMDS_ARRAY;
