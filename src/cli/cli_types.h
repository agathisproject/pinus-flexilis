// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLI_TYPES_BEES5G972K9T2FRP
#define CLI_TYPES_BEES5G972K9T2FRP
/** @file */

#include <stdint.h>

#define CLI_BUFF_SIZE 32  /**< max command size */
#define CLI_WORD_CNT 4    /**< number of words for a command */
#define CLI_WORD_SIZE 12  /**< max command word size */

typedef struct {
    char cmd[CLI_WORD_SIZE];
    uint8_t nParams;
    char params[CLI_WORD_CNT][CLI_WORD_SIZE];
} ParsedCmd_t;

typedef enum {
    CMD_DONE,
    CMD_WRONG_N,
    CMD_WRONG_PARAM,
} CliCmdReturn_t;

typedef struct {
    char context[CLI_WORD_SIZE];
} CliEnv;

#define CMD_ARG_DESC_SIZE 32
#define CMD_HELP_SIZE 32

typedef struct {
    char cmd[CLI_WORD_SIZE]; /**< command */
    char argDesc[CMD_ARG_DESC_SIZE]; /**< argument description */
    char cmdHelp[CMD_HELP_SIZE]; /**< command help/description */
    CliCmdReturn_t (*fptr)(ParsedCmd_t *cmdp);
} CliCmd_t;

#endif /* CLI_TYPES_BEES5G972K9T2FRP */
