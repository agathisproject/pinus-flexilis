// SPDX-License-Identifier: GPL-3.0-or-later

#include "tasks.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "hw/platform_esp/base.h"
#elif defined(__linux__)
#include "hw/platform_sim/base.h"
#include "sim/state.h"
#endif

#include "agathis/base.h"
#include "agathis/comm.h"
#include "cli/cli.h"
#include "cmd.h"
#include "hw/misc.h"

static CLICmdDef_t p_cmd_root[3]  = {
    {"info", "", "show module info", &cmd_info},
    {"set",  "", "change configuration", &cmd_set},
    {"save", "", "save configuration", &cmd_save},
};
static CLICmdFolder_t p_f_root = {"", sizeof(p_cmd_root) / sizeof(p_cmd_root[0]), p_cmd_root,
                                  NULL, NULL, NULL, NULL, NULL
                                 };

static CLICmdDef_t p_cmd_mod[5]  = {
    {"info", "", "show modules", &cmd_mod_info},
    {"id", "", "identify module", &cmd_mod_id},
    {"reset", "", "reset module", &cmd_mod_reset},
    {"on", "", "power on module", &cmd_mod_power_on},
    {"off", "", "power off module", &cmd_mod_power_off},
};
static CLICmdFolder_t p_f_mod = {"mod", sizeof(p_cmd_mod) / sizeof(p_cmd_mod[0]), p_cmd_mod,
                                 &p_cmd_mod[0], NULL, NULL, NULL, NULL
                                };

static CLICmdFolder_t p_f_lcl = {"lcl", 0, NULL, NULL, NULL, NULL, NULL, NULL};

#if MOD_HAS_PWR
static CLICmdDef_t p_cmd_pwr[4]  = {
    {"stats",  "", "show stats", &cmd_pwr_stats},
    {"ctrl",   "", "show controls", &cmd_pwr_ctrl},
    {"v5_src",  "[on|off]", "V5 source switch", &cmd_pwr_v5_src},
    {"v5_load", "[on|off]", "V5 load switch", &cmd_pwr_v5_load},
};
static CLICmdFolder_t p_f_pwr  = {"pwr", sizeof(p_cmd_pwr) / sizeof(p_cmd_pwr[0]), p_cmd_pwr,
                                  &p_cmd_pwr[0], NULL, NULL, NULL, NULL
                                 };
#else
static CLICmdFolder_t p_f_pwr  = {"pwr", 0, NULL, NULL, NULL, NULL, NULL, NULL};
#endif

static CLICmdFolder_t p_f_clk  = {"clk", 0, NULL, NULL, NULL, NULL, NULL, NULL};
static CLICmdFolder_t p_f_pps  = {"pps", 0, NULL, NULL, NULL, NULL, NULL, NULL};
static CLICmdFolder_t p_f_jtag = {"jtag", 0, NULL, NULL, NULL, NULL, NULL, NULL};
static CLICmdFolder_t p_f_usb  = {"usb", 0, NULL, NULL, NULL, NULL, NULL, NULL};
static CLICmdFolder_t p_f_pcie = {"pcie", 0, NULL, NULL, NULL, NULL, NULL, NULL};

static void p_CLIInit(void) {
    p_f_root.parent = &p_f_root;
    p_f_root.child = &p_f_lcl;

    p_f_lcl.parent = &p_f_root;
    p_f_lcl.right = &p_f_mod;
    p_f_lcl.child = &p_f_pwr;

    p_f_mod.parent = &p_f_root;
    p_f_mod.left = &p_f_lcl;

    p_f_pwr.parent = &p_f_lcl;
    p_f_pwr.right = &p_f_clk;

    p_f_clk.parent = &p_f_lcl;
    p_f_clk.left = &p_f_pwr;
    p_f_clk.right = &p_f_pps;

    p_f_pps.parent = &p_f_lcl;
    p_f_pps.left = &p_f_clk;
    p_f_pps.right = &p_f_jtag;

    p_f_jtag.parent = &p_f_lcl;
    p_f_jtag.left = &p_f_pps;
    p_f_jtag.right = &p_f_usb;

    p_f_usb.parent = &p_f_lcl;
    p_f_usb.left = &p_f_jtag;
    p_f_usb.right = &p_f_pcie;

    p_f_pcie.parent = &p_f_lcl;
    p_f_pcie.left = &p_f_usb;

    char prompt[CLI_PROMPT_MAX_LEN];
    uint32_t mac[2];

    get_HW_ID_compact(mac);
#if defined(ESP_PLATFORM)
    snprintf(prompt, CLI_PROMPT_MAX_LEN, "[%06lx:%06lx]$ ", mac[1], mac[0]);
#elif defined(__linux__)
    snprintf(prompt, CLI_PROMPT_MAX_LEN, "[%06x:%06x]$ ", mac[1], mac[0]);
#endif
    cli_SetPrompt(prompt);
    cli_Init(&p_f_root);

    gpio_RGB_send(0);
}

#if defined(ESP_PLATFORM)
void task_cli(void *pvParameter) {
    uint8_t parseSts;

    p_CLI_init_prompt();
    while (1) {
        printf("%s", CLI_getPrompt());
        CLI_getCmd();
        parseSts = CLI_parseCmd();
        if (parseSts == 0) {
            CLI_execute();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
#elif defined(__linux__)
void *task_cli (void *vargp) {
    if ((SIM_STATE.sim_flags & SIM_FLAG_NO_CONSOLE) != 0) {
        while (1) {
            sleep(1);
        }
    } else {
        p_CLIInit();
        while (1) {
            //printf("%s", cli_GetPrompt());
            cli_Read();
            cli_Parse();
            cli_Execute();
        }
    }
}
#endif

#if defined(ESP_PLATFORM)
void task_rf(void *pvParameter) {
    //char *appName = pcTaskGetName(NULL);
    ag_comm_init();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    while (1) {
        ag_comm_main();
        ag_upd_remote_mods();
        ag_upd_alarm();
        ag_upd_hw();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
#elif defined(__linux__)
void *task_rf (void *vargp) {
    ag_comm_init();

    while (1) {
        ag_comm_main();
        ag_upd_remote_mods();
        ag_upd_alarm();
        ag_upd_hw();
        sleep(1);
    }
}
#endif
