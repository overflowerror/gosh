#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../global.h"

typedef int error_t;

typedef struct {
	error_t last;
} state_t;

typedef int (*program_handler_t)(int, char**);

typedef struct {
	const char* name;
	program_handler_t handler; 
} program_t;

void interpreter_addBuiltIn(const char*, program_handler_t);

void interpreter_init(settings_t*);
state_t interpreter_initState();

int interpreter_do(state_t*, char*);

void builtin_add();

#endif
