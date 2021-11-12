// SPDX-License-Identifier: GPL-3.0-or-later

#include "cli_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../agathis/config.h"
#include "../agathis/base.h"

enum {
    CMD_QUESTION = 0,
    CMD_TFUN,
    CMD_DEV,
    CMD_MOD,
    CMD_PWR,
    CMD_V5_SRC,
    CMD_V5_LOAD,
#if MOD_HAS_CLK
    CMD_CLK,
#endif
#if MOD_HAS_1PPS
    CMD_1PPS,
#endif
#if MOD_HAS_JTAG
    CMD_JTAG,
#endif
#if MOD_HAS_USB
    CMD_USB,
#endif
#if MOD_HAS_PCIE
    CMD_PCIE,
#endif
    CMD_CNT
};

unsigned int CLI_getCmdCnt(void) {
    return CMD_CNT;
}

CLI_CMD_RETURN_t info(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t tfun(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t dev(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t mod(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t pwr(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t v5_src(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t v5_load(CLI_PARSED_CMD_t *cmdp);
#if MOD_HAS_CLK
CLI_CMD_RETURN_t clk(CLI_PARSED_CMD_t *cmdp);
#endif
#if MOD_HAS_1PPS
CLI_CMD_RETURN_t pps(CLI_PARSED_CMD_t *cmdp);
#endif
#if MOD_HAS_JTAG
CLI_CMD_RETURN_t jtag(CLI_PARSED_CMD_t *cmdp);
#endif

static CLI_CMD_t p_CMDS_ARRAY[CMD_CNT] = {
    {"?", "", "show module info", 0, 0, &info},
    {"tfun", "", "trunk functions", 0, 1, &tfun},
    {"dev", "", "show devices", 0, 0, &dev},
    {"mod", "", "show modules", 0, 0, &mod},
    {"pwr", "", "power", 1, 2, &pwr},
    {"v5_src", "[on|off]", "V5 source switch", 2, 2, &v5_src},
    {"v5_load", "[on|off]", "V5 load switch", 2, 2, &v5_load},
#if MOD_HAS_CLK
    {"clk", "", "clock", 1, 1, &clk},
#endif
#if MOD_HAS_1PPS
    {"1pps", "", "pulse per sec", 1, 1, &pps},
#endif
#if MOD_HAS_JTAG
    {"jtag", "", "JTAG", 1, 1, &jtag},
#endif
};

CLI_CMD_t *CMDS = p_CMDS_ARRAY;

CLI_CMD_RETURN_t info(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    if (MOD_STATE.caps_ext & AG_CAP_EXT_TMC) {
        printf("MC_TYPE = TMC\n");
    } else {
        printf("MC_TYPE = MMC\n");
    }
    printf("MC_ADDR = %d\n", MOD_STATE.addr_d);
    printf("MFR_NAME = %s\n", MOD_STATE.mfr_name);
    printf("MFR_PN = %s\n", MOD_STATE.mfr_pn);
    printf("MFR_SN = %s\n", MOD_STATE.mfr_sn);
    return CMD_DONE;
}

CLI_CMD_RETURN_t tfun(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

//    for (unsigned int i = 0; i < CLI_getCmdCnt(); i++) {
//        if (CMDS[i].group == 1) {
//            printf("%s\n", CMDS[i].cmd);
//        }
//    }
    return CMD_DONE;
}

CLI_CMD_RETURN_t dev(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("dev1\n");
    printf("dev2\n");
    return CMD_DONE;
}

CLI_CMD_RETURN_t mod(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    if (!(MOD_STATE.caps_ext & AG_CAP_EXT_TMC)) {
        return CMD_DONE;
    }

    printf("mod1\n");
    printf("mod2\n");
    printf("mod3\n");
    return CMD_DONE;
}

CLI_CMD_RETURN_t pwr(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("V5_SRC = OFF\n");
    printf("V5_LOAD = OFF\n");
    printf("I5 = %.3f A\n", ag_get_I5_NOM());
    printf("I3 = %.3f A\n", ag_get_I3_NOM());

    printf("SRC_I5_NOM = %.3f A\n", MOD_STATE.i5_nom);
    printf("SRC_I5_CUTOFF = %.3f A\n", MOD_STATE.i5_cutoff);
    printf("SRC_I3_NOM = %.3f A\n", MOD_STATE.i3_nom);
    printf("SRC_I3_CUTOFF = %.3f A\n", MOD_STATE.i3_cutoff);

    printf("LOAD_I5_NOM = %.3f A\n", MOD_STATE.i5_nom);
    printf("LOAD_I5_CUTOFF = %.3f A\n", MOD_STATE.i5_cutoff);
    printf("LOAD_I3_NOM = %.3f A\n", MOD_STATE.i3_nom);
    return CMD_DONE;
}

CLI_CMD_RETURN_t v5_src(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 1) {
        return CMD_WRONG_N;
    }

    if (strncmp(cmdp->params[0], "on", 2) == 0) {
        printf("on\n");
    } else if (strncmp(cmdp->params[0], "off", 3) == 0) {
        printf("off\n");
    } else {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}

CLI_CMD_RETURN_t v5_load(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 1) {
        return CMD_WRONG_N;
    }

    if (strncmp(cmdp->params[0], "on", 2) == 0) {
        printf("on\n");
    } else if (strncmp(cmdp->params[0], "off", 3) == 0) {
        printf("off\n");
    } else {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}

#if MOD_HAS_CLK
CLI_CMD_RETURN_t clk(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}
#endif

#if MOD_HAS_1PPS
CLI_CMD_RETURN_t pps(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}
#endif

#if MOD_HAS_JTAG
CLI_CMD_RETURN_t jtag(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    return CMD_DONE;
}
#endif
