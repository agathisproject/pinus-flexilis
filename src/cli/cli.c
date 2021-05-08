// SPDX-License-Identifier: GPL-3.0-or-later

#include "cli.h"
#include "cli_types.h"

#include <stdio.h>
#include <string.h>

#ifdef __AVR__
#include "../mcc_generated_files/uart1.h"
#elif __linux__
uint8_t UART1_IsRxReady(void) {
    return 1;
}
uint8_t UART1_Read(void) {
    return (uint8_t) getchar();
}
#endif

static char _CLI_BUFF[CLI_BUFF_SIZE + 1] = "\0";

static ParsedCmd_t _PARSED_CMD = {"\0", 0, {"\0", "\0", "\0", "\0"}};

void CLI_Get_Cmd() {
    uint8_t byteIn;
    uint8_t idx = 0;

    strncpy(_CLI_BUFF, "\0", (CLI_BUFF_SIZE + 1));
    while (1) {
        if (UART1_IsRxReady()) {
            byteIn = UART1_Read();
            //printf("%x\n", byteIn);

            // NL or CR end the command
            if ((byteIn == 10) || (byteIn == 13)) {
                break;
            }
            // BS removes last character from buffer
            if (byteIn == 8) {
                if (idx > 0) {
                    idx --;
                    _CLI_BUFF[idx] = '\0';
                }
                continue;
            }
            // skip non-printable characters
            if ((byteIn < 32) || (byteIn > 126)) {
                break;
            }
            // if buffer full skip adding
            if (idx == CLI_BUFF_SIZE) {
                continue;
            }
            _CLI_BUFF[idx] = (char)byteIn;
            idx ++;
            _CLI_BUFF[idx] = '\0';
        }
    }
}

uint8_t CLI_Parse_Cmd() {
    strncpy(_PARSED_CMD.cmd, "\0", CLI_PARAM_SIZE);
    _PARSED_CMD.nParams = 0;
    for (uint8_t i = 0; i < CLI_PARAM_CNT; i++) {
        strncpy(_PARSED_CMD.params[i], "\0", CLI_PARAM_SIZE);
    }

    //printf("DBG: parse ...\n");
    uint8_t j = 0;
    for (uint8_t i = 0; i < CLI_BUFF_SIZE; i ++) {
        if (_CLI_BUFF[i] == 32) {
            _PARSED_CMD.nParams ++;
            j = 0;
            continue;
        }
        if (_CLI_BUFF[i] == 0) {
            _PARSED_CMD.nParams ++;
            break;
        }

        if (j >= CLI_PARAM_SIZE) {
            printf("ERROR parsing (size)\n");
            return 1;
        }
        if (_PARSED_CMD.nParams >= CLI_PARAM_CNT) {
            printf("ERROR parsing (param)\n");
            return 2;
        }

        if (_PARSED_CMD.nParams == 0) {
            _PARSED_CMD.cmd[j] = _CLI_BUFF[i];
            j ++;
            _PARSED_CMD.cmd[j] = '\0';
        } else {
            _PARSED_CMD.params[_PARSED_CMD.nParams - 1][j] = _CLI_BUFF[i];
            j ++;
            _PARSED_CMD.params[_PARSED_CMD.nParams - 1][j] = '\0';
        }
    }
    if (_PARSED_CMD.nParams > 0) {
        _PARSED_CMD.nParams --;
    }
    return 0;
}

void CLI_Execute() {
    //printf("DBG: execute %s (%d params)\n", _PARSED_CMD.cmd, _PARSED_CMD.nParams);
    if (strlen(_PARSED_CMD.cmd) == 0) {
        return;
    }

    unsigned int i = 0;
    if (strncmp(_PARSED_CMD.cmd, "help", CLI_PARAM_SIZE) == 0) {
        for (i = 0; i < Get_Cmd_Cnt(); i++) {
            printf("  %s - %s\n", CMDS[i].cmd, CMDS[i].help);
        }
        return;
    }

    CliCmdReturn_t cmdRet = CMD_DONE;
    for (i = 0; i < Get_Cmd_Cnt(); i++) {
        if (strncmp(_PARSED_CMD.cmd, CMDS[i].cmd, CLI_PARAM_SIZE) == 0) {
            cmdRet = CMDS[i].fptr(&_PARSED_CMD);
            break;
        }
    }

    if (i == Get_Cmd_Cnt()) {
        printf("UNRECOGNIZED command\n");
    } else if (cmdRet == CMD_WRONG_N) {
        printf("WRONG argument count\n");
        printf("  %s %s\n", CMDS[i].cmd, CMDS[i].desc);
    } else if (cmdRet == CMD_WRONG_PARAM) {
        printf("WRONG arguments\n");
        printf("  %s %s\n", CMDS[i].cmd, CMDS[i].desc);
    }
}
