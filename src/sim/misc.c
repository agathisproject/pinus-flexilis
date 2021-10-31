// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdlib.h>
#include <stdio.h>

#include "misc.h"

float getValue_random(float val, int maxPercent) {
    float tmp = (float)((rand() % (2 * maxPercent)) - maxPercent) / 100.0f;
    return val * (1.0f + tmp);
}
