// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CMD_H868VY2FS9UU4CP6
#define CMD_H868VY2FS9UU4CP6
/** @file */

#include <stdint.h>

#include "cli_types.h"
#include "../agathis/config.h"

CLI_CMD_RETURN_t cmd_info(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t cmd_dev(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t cmd_mod(CLI_PARSED_CMD_t *cmdp);
#if MOD_HAS_PWR
CLI_CMD_RETURN_t cmd_pwr_stats(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t cmd_pwr_ctrl(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t cmd_pwr_v5_src(CLI_PARSED_CMD_t *cmdp);
CLI_CMD_RETURN_t cmd_pwr_v5_load(CLI_PARSED_CMD_t *cmdp);
#endif

#endif /* CMD_H868VY2FS9UU4CP6 */
