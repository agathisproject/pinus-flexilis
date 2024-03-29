// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AGATHIS_98RXG9U8BUUHY401
#define AGATHIS_98RXG9U8BUUHY401

typedef enum {
    MC_CMD_FAIL,
    MC_CMD_OK,
    MC_CMD_PASS,
} AG_MC_CMD_STATUS_t;

#define AG_CAP_INT_EEPROM    0x01

#define AG_PROTO_VER         1

#define AG_CMD_SUMMARY       0x01
#define AG_CMD_SUMMARY_NB    4
#define AG_FLAG_RESET        0x01

#define AG_CMD_CAP           0x02
#define AG_CMD_CAP_NB        4
#define AG_CAP_EXT_PCIE      0x40
#define AG_CAP_EXT_USB       0x20
#define AG_CAP_EXT_JTAG      0x10
#define AG_CAP_EXT_1PPS      0x08
#define AG_CAP_EXT_CLK       0x04
#define AG_CAP_EXT_PWR       0x02
#define AG_CAP_SW_TMC        0x01

#define AG_CMD_MFR          0xFE
#define AG_CMD_MFR_NB       4

#endif /* AGATHIS_98RXG9U8BUUHY401 */
