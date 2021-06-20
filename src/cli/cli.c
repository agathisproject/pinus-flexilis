// SPDX-License-Identifier: GPL-3.0-or-later

#include "cli.h"

#include <stdio.h>
#include <string.h>

#if defined(__AVR__)
#include "../mcc_generated_files/uart1.h"

uint8_t CLI_IsRxReady(void) {
    return UART1_IsRxReady();
}

uint8_t CLI_GetChar(void) {
    return (uint8_t) UART1_Read();
}
#elif defined(__linux__) || defined(__MINGW64__)
uint8_t p_CLI_IsRxReady(void) {
    return 1;
}

uint8_t p_CLI_GetChar(void) {
    return (uint8_t) getchar();
}
#endif

static char p_CLI_BUFF[CLI_BUFF_SIZE + 1] = { 0 };
static char p_CLI_PROMPT[CLI_PROMPT_SIZE] = { 0 };

static CLI_PARSED_CMD_t p_PARSED_CMD = {"\0", 0, {"", "", "", ""}};

static CLI_ENV_t p_CLI_ENV = {{0, 0, 0, 0, 0, 0, 0, 0}, 0};

char * CLI_getPrompt(void) {
    return p_CLI_PROMPT;
}

void CLI_setPrompt(const char *str) {
    strncpy(p_CLI_PROMPT, str, CLI_PROMPT_SIZE);
}

void p_Make_Prompt(void) {
    memset(p_CLI_PROMPT, 0, CLI_PROMPT_SIZE);

    if (p_CLI_ENV.groupStack[p_CLI_ENV.groupStackIdx] != 0) {
        strncat(p_CLI_PROMPT, "up ", (CLI_PROMPT_SIZE - 1));
    }

    for (unsigned int i = 0; i < CLI_getCmdCnt(); i++) {
        if (CMDS[i].group == p_CLI_ENV.groupStack[p_CLI_ENV.groupStackIdx]) {
            strncat(p_CLI_PROMPT, CMDS[i].cmd, (CLI_PROMPT_SIZE - 1));
            strncat(p_CLI_PROMPT, " ", (CLI_PROMPT_SIZE - 1));
        }
    }
    if (strlen(p_CLI_PROMPT) <= (CLI_PROMPT_SIZE - 2)) {
        strncat(p_CLI_PROMPT, ">", (CLI_PROMPT_SIZE - 1));
    } else {
        p_CLI_PROMPT[CLI_PROMPT_SIZE - 2] = '>';
        p_CLI_PROMPT[CLI_PROMPT_SIZE - 1] = '\0';
    }
}

void CLI_init(void) {
    p_Make_Prompt();
}

void CLI_getCmd(void) {
    uint8_t byteIn;
    uint8_t idx = 0;

    strncpy(p_CLI_BUFF, "\0", (CLI_BUFF_SIZE + 1));
    while (1) {
        if (p_CLI_IsRxReady()) {
            byteIn = p_CLI_GetChar();
            //printf("%x\n", byteIn);

            // NL or CR end the command
            if ((byteIn == 10) || (byteIn == 13)) {
                break;
            }
            // BS removes last character from buffer
            if (byteIn == 8) {
                if (idx > 0) {
                    idx --;
                    p_CLI_BUFF[idx] = '\0';
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
            p_CLI_BUFF[idx] = (char)byteIn;
            idx ++;
            p_CLI_BUFF[idx] = '\0';
        }
    }
}

uint8_t CLI_parseCmd(void) {
    strncpy(p_PARSED_CMD.cmd, "\0", CLI_WORD_SIZE);
    p_PARSED_CMD.nParams = 0;
    for (uint8_t i = 0; i < CLI_WORD_CNT; i++) {
        strncpy(p_PARSED_CMD.params[i], "\0", CLI_WORD_SIZE);
    }

    //printf("DBG: parse ...\n");
    uint8_t j = 0;
    for (uint8_t i = 0; i < CLI_BUFF_SIZE; i ++) {
        if (p_CLI_BUFF[i] == 32) {
            p_PARSED_CMD.nParams ++;
            j = 0;
            continue;
        }
        if (p_CLI_BUFF[i] == 0) {
            p_PARSED_CMD.nParams ++;
            break;
        }

        if (j >= CLI_WORD_SIZE) {
            printf("ERROR parsing (size)\n");
            return 1;
        }
        if (p_PARSED_CMD.nParams >= CLI_WORD_CNT) {
            printf("ERROR parsing (param)\n");
            return 2;
        }

        if (p_PARSED_CMD.nParams == 0) {
            p_PARSED_CMD.cmd[j] = p_CLI_BUFF[i];
            j ++;
            p_PARSED_CMD.cmd[j] = '\0';
        } else {
            p_PARSED_CMD.params[p_PARSED_CMD.nParams - 1][j] = p_CLI_BUFF[i];
            j ++;
            p_PARSED_CMD.params[p_PARSED_CMD.nParams - 1][j] = '\0';
        }
    }
    if (p_PARSED_CMD.nParams > 0) {
        p_PARSED_CMD.nParams --;
    }
    return 0;
}

void CLI_execute(void) {
    //printf("DBG: execute %s (%d params) %d\n", p_PARSED_CMD.cmd, p_PARSED_CMD.nParams, p_CLI_ENV.group);
    if (strlen(p_PARSED_CMD.cmd) == 0) {
        return;
    }

    unsigned int i = 0;
//    if (strncmp(p_PARSED_CMD.cmd, "help", 4) == 0) {
//        for (i = 0; i < CLI_getCmdCnt(); i++) {
//            if (CMDS[i].cmd[0] != '.') {
//                printf("  %s - %s\n", CMDS[i].cmd, CMDS[i].cmdHelp);
//            }
//        }
//        return;
//    }

    if (strncmp(p_PARSED_CMD.cmd, "up", 2) == 0) {
        if (p_CLI_ENV.groupStackIdx > 0) {
            p_CLI_ENV.groupStackIdx --;
        } else {
            printf("ERROR: CLI groupStack under-flow\n");
        };

        p_Make_Prompt();
        return;
    }

    CLI_CMD_RETURN_t cmdRet = CMD_DONE;
    for (i = 0; i < CLI_getCmdCnt(); i++) {
        if ((strncmp(p_PARSED_CMD.cmd, CMDS[i].cmd, CLI_WORD_SIZE) == 0)
                && (p_CLI_ENV.groupStack[p_CLI_ENV.groupStackIdx] == CMDS[i].group)) {
            cmdRet = CMDS[i].fptr(&p_PARSED_CMD);
            break;
        }
    }

    if (i == CLI_getCmdCnt()) {
        printf("UNRECOGNIZED command\n");
    } else if (cmdRet == CMD_WRONG_N) {
        printf("WRONG argument count\n");
        printf("  %s %s\n", CMDS[i].cmd, CMDS[i].argDesc);
    } else if (cmdRet == CMD_WRONG_PARAM) {
        printf("WRONG arguments\n");
        printf("  %s %s\n", CMDS[i].cmd, CMDS[i].argDesc);
    } else {
        if (CMDS[i].next_group != p_CLI_ENV.groupStack[p_CLI_ENV.groupStackIdx]) {
            if (p_CLI_ENV.groupStackIdx < (CLI_ENV_GRP_STACK_SIZE - 1)) {
                p_CLI_ENV.groupStackIdx ++;
                p_CLI_ENV.groupStack[p_CLI_ENV.groupStackIdx] = CMDS[i].next_group;
            } else {
                printf("ERROR: CLI groupStack over-flow\n");
            }
        }
    }
    p_Make_Prompt();
}
