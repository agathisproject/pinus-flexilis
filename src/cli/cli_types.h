// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLI_TYPES_BEES5G972K9T2FRP
#define CLI_TYPES_BEES5G972K9T2FRP
/** @file */

#include <stdint.h>

#define CLI_BUFF_SIZE 32    /**< max command size */
#define CLI_WORD_CNT 4      /**< number of words for a command */
#define CLI_WORD_SIZE 12    /**< max command word size */
#define CLI_PROMPT_SIZE 32  /**< max prompt size */

typedef struct {
    char cmd[CLI_WORD_SIZE];
    uint8_t nParams;
    char params[CLI_WORD_CNT][CLI_WORD_SIZE];
} CLI_PARSED_CMD_t;

typedef enum {
    CMD_DONE,
    CMD_WRONG_N,
    CMD_WRONG_PARAM,
} CLI_CMD_RETURN_t;

#define CMD_ARG_DESC_SIZE 32
#define CMD_HELP_SIZE 32

typedef struct {
    const char cmd[CLI_WORD_SIZE]; /**< command */
    const char argDesc[CMD_ARG_DESC_SIZE]; /**< argument description */
    const char cmdHelp[CMD_HELP_SIZE]; /**< command help/description */
    uint8_t in_group;
    uint8_t next_group;
    CLI_CMD_RETURN_t (*fptr)(CLI_PARSED_CMD_t *cmdp);
} CLI_CMD_t;

#endif /* CLI_TYPES_BEES5G972K9T2FRP */
