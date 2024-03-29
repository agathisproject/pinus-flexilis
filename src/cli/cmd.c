// SPDX-License-Identifier: GPL-3.0-or-later

#include "cmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../agathis/base.h"
#include "../agathis/cmds.h"
#include "../agathis/config.h"
#include "../hw/storage.h"

CLI_CMD_RETURN_t cmd_info(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("caps = %#04x/%#04x/%#04x\n", MOD_STATE.caps_hw_ext,
           MOD_STATE.caps_hw_int, MOD_STATE.caps_sw);
    printf("MFR_NAME = %s\n", MOD_STATE.mfr_name);
    printf("MFR_PN = %s\n", MOD_STATE.mfr_pn);
    printf("MFR_SN = %s\n", MOD_STATE.mfr_sn);
    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_dev(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("dev1\n");
    printf("dev2\n");
    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_set(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 2) {
        return CMD_WRONG_N;
    }

    if (strncmp(cmdp->params[0], "master", 6) == 0) {
        if (strncmp(cmdp->params[1], "on", 2) == 0) {
            MOD_STATE.caps_sw |= AG_CAP_SW_TMC;
        } else {
            MOD_STATE.caps_sw &= (uint8_t) (~AG_CAP_SW_TMC);
        }
    }

    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_save(CLI_PARSED_CMD_t *cmdp) {
    stor_save_state();
    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_mod_info(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    for (int i = 0; i < MC_MAX_CNT; i ++) {
        if (REMOTE_MODS[i].last_seen == -1) {
            continue;
        }
        printf("%06x:%06x (%d)\n", REMOTE_MODS[i].mac[1], REMOTE_MODS[i].mac[0],
               REMOTE_MODS[i].last_seen);
    }
    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_pwr_stats(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("V5_SRC = OFF\n");
    printf("V5_LOAD = OFF\n");
    printf("I5 = %.3f A\n", ag_get_I5_NOM());
    printf("I3 = %.3f A\n", ag_get_I3_NOM());
    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_pwr_ctrl(CLI_PARSED_CMD_t *cmdp) {
    if (cmdp->nParams != 0) {
        return CMD_WRONG_N;
    }

    printf("SRC_I5_NOM = %.3f A\n", MOD_STATE.i5_nom);
    printf("SRC_I5_CUTOFF = %.3f A\n", MOD_STATE.i5_cutoff);
    printf("SRC_I3_NOM = %.3f A\n", MOD_STATE.i3_nom);
    printf("SRC_I3_CUTOFF = %.3f A\n", MOD_STATE.i3_cutoff);

    printf("LOAD_I5_NOM = %.3f A\n", MOD_STATE.i5_nom);
    printf("LOAD_I5_CUTOFF = %.3f A\n", MOD_STATE.i5_cutoff);
    printf("LOAD_I3_NOM = %.3f A\n", MOD_STATE.i3_nom);
    return CMD_DONE;
}

CLI_CMD_RETURN_t cmd_pwr_v5_src(CLI_PARSED_CMD_t *cmdp) {
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

CLI_CMD_RETURN_t cmd_pwr_v5_load(CLI_PARSED_CMD_t *cmdp) {
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
