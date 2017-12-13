#ifndef CLI_H
#define CLI_H

#include "global.h"
#include "interpreter/interpreter.h"

void cli_init(settings_t*);

char* cli_getLine(state_t*);

void cli_afterLine(char*);

#endif
