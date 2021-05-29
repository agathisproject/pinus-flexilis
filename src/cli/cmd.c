// SPDX-License-Identifier: GPL-3.0-or-later

#include "cli_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../agathis/base.h"

CLI_CMD_RETURN_t p_env(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams == 0) {
        printf("CLI\n");
        return CMD_DONE;
    } else if (cmdp->nParams == 3) {
        return CMD_DONE;
    } else {
        return CMD_WRONG_N;
    }
}

CLI_CMD_RETURN_t info(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    if (MOD_STATE.flags & AG_FLAG_TMC) {
        printf("TMC, addr %d\n", MOD_STATE.addr_d);
    } else {
        printf("MMC, addr %d\n", MOD_STATE.addr_d);
    }
    printf("MFR_NAME\n");
    printf("MFR_PN\n");
    printf("MFR_SN\n");
    return CMD_DONE;
}

CLI_CMD_RETURN_t dev(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("pwr\n");
    printf("jtag\n");
    printf("clk\n");
    printf("1pps\n");
    return CMD_DONE;
}

CLI_CMD_RETURN_t mod(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("mod1\n");
    printf("mod2\n");
    printf("mod3\n");
    return CMD_DONE;
}

CLI_CMD_RETURN_t gpio(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}

#define CMD_CNT 7

unsigned int CLI_getCmdCnt(void) {
    return CMD_CNT;
}

static CLI_CMD_t p_CMDS_ARRAY[CMD_CNT] = {
    {"?", "", "show module info", 0, 0, &info},
    {"dev", "", "show devices", 0, 1, &dev},
    {"mod", "", "show modules", 0, 0, &mod},
    {"pwr", "", "device", 1, 1, &gpio},
    {"clk", "", "device", 1, 1, &gpio},
    {"1pps", "", "device", 1, 1, &gpio},
    {"jtag", "", "device", 1, 1, &gpio},
};

CLI_CMD_t *CMDS = p_CMDS_ARRAY;
