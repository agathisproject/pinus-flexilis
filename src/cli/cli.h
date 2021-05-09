// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLI_YP8GJVG3TCW7BWQW
#define CLI_YP8GJVG3TCW7BWQW
/** @file */

#include <stdint.h>

#define CLI_PROMPT "> "

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

#endif /* CLI_YP8GJVG3TCW7BWQW */
