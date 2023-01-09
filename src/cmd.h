// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CMD_H868VY2FS9UU4CP6
#define CMD_H868VY2FS9UU4CP6
/** @file */

#include <stdint.h>

#include "cli/cli_types.h"
#include "agathis/config.h"

CLIStatus_t cmd_info(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_set(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_save(CLICmdParsed_t *cmdp);

CLIStatus_t cmd_mod_info(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_mod_id(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_mod_reset(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_mod_power_on(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_mod_power_off(CLICmdParsed_t *cmdp);
#if MOD_HAS_PWR
CLIStatus_t cmd_pwr_stats(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_pwr_ctrl(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_pwr_v5_src(CLICmdParsed_t *cmdp);
CLIStatus_t cmd_pwr_v5_load(CLICmdParsed_t *cmdp);
#endif

#endif /* CMD_H868VY2FS9UU4CP6 */
