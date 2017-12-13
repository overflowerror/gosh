#include <stdio.h>

#include "interpreter.h"
#include "built-in/builtin.h"

int hello_world(int argc, char** argv) {

	printf("Hello World\n");

	return 0;
}

void builtin_add() {
	interpreter_addBuiltIn("hello", hello_world);
	interpreter_addBuiltIn("cd", cd);
}
