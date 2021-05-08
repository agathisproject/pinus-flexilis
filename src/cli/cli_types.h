// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLI_TYPES_BEES5G972K9T2FRP
#define CLI_TYPES_BEES5G972K9T2FRP
/** @file */

#include <stdint.h>

#define CLI_BUFF_SIZE 32
#define CLI_PARAM_CNT 4 /**< number of parameters for a command */
#define CLI_PARAM_SIZE 12

typedef struct {
    char cmd[CLI_PARAM_SIZE];
    uint8_t nParams;
    char params[CLI_PARAM_CNT][CLI_PARAM_SIZE];
} ParsedCmd_t;

#define CMD_DESC_SIZE 32
#define CMD_HELP_SIZE 32

typedef enum {
    CMD_DONE,
    CMD_WRONG_N,
    CMD_WRONG_PARAM,
} CliCmdReturn_t;

typedef struct {
    char cmd[CLI_PARAM_SIZE];
    char desc[CMD_DESC_SIZE];
    char help[CMD_HELP_SIZE];
    CliCmdReturn_t (*fptr)(ParsedCmd_t *cmdp);
} CliCmd_t;

extern CliCmd_t *CMDS;
unsigned int Get_Cmd_Cnt();

#endif /* CLI_TYPES_BEES5G972K9T2FRP */
