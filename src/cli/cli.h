// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLI_YP8GJVG3TCW7BWQW
#define CLI_YP8GJVG3TCW7BWQW
/** @file */

#include <stdint.h>

#include "cli_types.h"

#define CLI_ENV_GRP_STACK_SIZE 8
typedef struct {
    uint8_t groupStack[CLI_ENV_GRP_STACK_SIZE];
    uint8_t groupStackIdx;
} CLI_ENV_t;

extern CLI_CMD_t *CMDS;

/**
 * @return get CLI prompt
 */
char * CLI_getPrompt(void);

/**
 * @brief set CLI prompt
 */
void CLI_setPrompt(const char *str);

/**
 * @brief init CLI library
 */
void CLI_init(void);

/**
 * @brief get command from UART/stdin into the internal buffer
 */
void CLI_getCmd(void);

/**
 * @brief parse command from internal buffer
 *
 * @return 0 is no errors
 */
uint8_t CLI_parseCmd(void);

/**
 * @brief execute command
 */
void CLI_execute(void);

/**
 *
 * @return
 */
unsigned int CLI_getCmdCnt(void);

#endif /* CLI_YP8GJVG3TCW7BWQW */
