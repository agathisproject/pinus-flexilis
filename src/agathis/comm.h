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

#endif /* AGATHIS_COMM_ZC5DS878HG83B98T */
