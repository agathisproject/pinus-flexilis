// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AGATHIS_COMM_ZC5DS878HG83B98T
#define AGATHIS_COMM_ZC5DS878HG83B98T
/** @file */

#include <stdint.h>

#if defined(__XC16__)
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#endif

#define AG_MSG_LEN 32

void ag_comm_init(void);

void ag_comm_main(void);

int ag_comm_tx(uint32_t dst_mac1, uint32_t dst_mac0, uint8_t *buff, uint8_t nb);

#endif /* AGATHIS_COMM_ZC5DS878HG83B98T */
