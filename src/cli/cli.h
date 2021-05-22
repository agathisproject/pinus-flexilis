// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLI_YP8GJVG3TCW7BWQW
#define CLI_YP8GJVG3TCW7BWQW
/** @file */

#include <stdint.h>

#include "cli_types.h"

typedef struct {
    uint8_t group;
    uint8_t prev_group;
} CliEnv;

/**
 * @return get CLI prompt
 */
char * CLI_Get_Prompt(void);

/**
 * @brief set CLI prompt
 */
void CLI_Set_Prompt(const char *str);

/**
 * @brief init CLI library
 */
void CLI_Init(void);

/**
 * @brief get command from UART/stdin into the internal buffer
 */
void CLI_Get_Cmd(void);

/**
 * @brief parse command from internal buffer
 *
 * @return 0 is no errors
 */
uint8_t CLI_Parse_Cmd(void);

/**
 * @brief execute command
 */
void CLI_Execute(void);

extern CliCmd_t *CMDS;
/**
 *
 * @return
 */
unsigned int Get_Cmd_Cnt(void);

#endif /* CLI_YP8GJVG3TCW7BWQW */
