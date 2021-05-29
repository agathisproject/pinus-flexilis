// SPDX-License-Identifier: GPL-3.0-or-later

#include "tmc.h"

#include <stdio.h>

#include "base.h"
/*
MCCmdStatus_t _check_id(uint8_t mc_id) {
    if (MC.TMC == 0) {
        printf("NOT on a TMC\n");
        return MC_CMD_FAIL;
    } else if (mc_id == 0) {
        return MC_CMD_OK;
    }
    return MC_CMD_PASS;
}

MCCmdStatus_t TMC_cmdID(uint8_t mc_id, uint8_t *reply) {
    MCCmdStatus_t sts = _check_id(mc_id);
    if (sts != MC_CMD_PASS) {
        return sts;
    }

    I2CXfer_t xfer = {I2C_XFER_NONE, 1, (I2C_OFFSET + mc_id)};

    SMBus_Rd_Block(&xfer, MC_CMD_ID, MC_CMD_ID_NB, reply);
    if (xfer.sts == I2C_XFER_OK) {
        return MC_CMD_OK;
    } else {
        return MC_CMD_FAIL;
    }
}

MCCmdStatus_t TMC_Show(uint8_t mc_id) {
    MCCmdStatus_t sts = _check_id(mc_id);
    if (sts != MC_CMD_PASS) {
        return sts;
    }

    if (RmtMC[mc_id - 1].state != MC_PRESENT) {
        printf("MC %d NOT PRESENT\n", mc_id);
        return MC_CMD_OK;
    }

    printf(" %4s: %d\n", "rst",
           ((RmtMC[mc_id - 1].pow_rst & MC_CMD_ID_RST_MASK) >> MC_CMD_ID_RST_OFFS) );
    printf(" %4s: %d\n", "pwr",
           ((RmtMC[mc_id - 1].pow_rst & MC_CMD_ID_PWR_MASK) >> MC_CMD_ID_PWR_OFFS) );
    return MC_CMD_OK;
}

MCCmdStatus_t TMC_Scan(void) {
    uint8_t data[MC_CMD_ID_NB] = {0, 0, 0, 0};
    MCCmdStatus_t sts = MC_CMD_FAIL;

    for (uint8_t i = 1; i < MC_MAX_CNT; i++) {
        sts = TMC_cmdID(i, (uint8_t *) &data);
        if (sts == MC_CMD_OK) {
            if (data[0] != (MC_CMD_ID_NB - 1)) {
                RmtMC[i - 1].state = MC_INVALID;
            } else {
                RmtMC[i - 1].state = MC_PRESENT;
                RmtMC[i - 1].pow_rst = data[1];
                RmtMC[i - 1].io = data[2];
            }
        } else {
            RmtMC[i - 1].state = MC_NOT_PRESENT;
        }
    }
    return MC_CMD_OK;
}
*/
