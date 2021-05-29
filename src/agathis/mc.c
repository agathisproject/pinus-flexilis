// SPDX-License-Identifier: GPL-3.0-or-later

#include "mc.h"

#include <stdio.h>

#include "base.h"
/*
void MC_cmdID_reply(uint8_t *i2c_buff) {
    i2c_buff[0] = 0x03;
    i2c_buff[1] = MC.pow_rst;
    i2c_buff[2] = MC.io;
    i2c_buff[3] = 0x00;
}

void MC_Show(void) {
    printf(" %4s: %d(d) %d(u) %#03x\n", "addr",
           MC.addr_d, MC.addr_u, MC.addr_i2c);
    printf(" %4s: %d\n", "TMC", MC.TMC);
    printf(" %4s: %d\n", "rst",
           ((MC.pow_rst & MC_CMD_ID_RST_MASK) >> MC_CMD_ID_RST_OFFS) );
    printf(" %4s: %d\n", "pwr",
           ((MC.pow_rst & MC_CMD_ID_PWR_MASK) >> MC_CMD_ID_PWR_OFFS) );
}

void MC_SetPower(uint8_t val) {
    if (val > 7) {
        printf("INCORRECT power state: %d\n", val);
        return;
    }

    MC.pow_rst &= ~MC_CMD_ID_PWR_MASK;
    MC.pow_rst |= ((val << MC_CMD_ID_PWR_OFFS) & MC_CMD_ID_PWR_MASK);
    // TODO: do HW actions
}
*/
