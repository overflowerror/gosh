#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


#include "../global.h"
#include "interpreter.h"
#include "splitter.h"

settings_t* settings;

int builtInCount = 0;
program_t* builtIn;

void interpreter_addBuiltIn(const char* name, program_handler_t handler) {
	void* tmp = realloc(builtIn, (builtInCount + 1) * sizeof(program_t));
	if (tmp == NULL)
		bailOut(EXIT_SETUP_FAILED, "interpreter_addBuiltIn", NULL);

	builtIn = tmp;
	
	builtIn[builtInCount] = (program_t) {
		.name = name,
		.handler = handler
	};

	builtInCount++;
}

void interpreter_init(settings_t* set) {
	settings = set;

	builtin_add();
}

state_t interpreter_initState() {
	return (state_t) {
		.last = 0
	};
}

program_handler_t interpreter_searchBuiltIn(char* name) {
	for (int i = 0; i < builtInCount; i++) {
		if (strcmp(builtIn[i].name, name) == 0)
			return builtIn[i].handler;
	}
	return NULL;
}

int interpreter_searchBin(char* name, char** file) {
	char* tmp = getenv("PATH");
	char* path = malloc(strlen(tmp) + 1);
	strcpy(path, tmp);
	tmp = path;

	char* token;
	
	int error = ENOENT;
	while((token = strtok(tmp, ":")) != NULL) {		
		tmp = NULL;

		*file = realloc(*file, strlen(token) + 1 + strlen(name) + 1);
		if (*file == NULL)
			bailOut(EXIT_FAILURE, "interpreter_searchBin", NULL);

		(void) strcpy(*file, token);
		(void) strcat(*file, "/");
		(void) strcat(*file, name);
		
		if (access(*file, R_OK | X_OK) < 0) {
			if (errno == EACCES)
				error = EACCES;
		} else {
			free(path);
			return 0;
		}
	}
	free(path);
	*file = NULL;
	return error;
}

error_t interpreter_startBuiltIn(program_handler_t handler, args_t args) {
	return handler(args.argc, args.argv);

	/*
	 * This is problematic: We cannot alter the env or the cwd of the shell-process from a child.
	 * TODO: Find a better solution.
	 */

	/*int pid = fork();
	if (pid < 0)
		bailOut(EXIT_FAILURE, "interpreter_forkBuiltIn", "fork");
	if (pid == 0) 
		exit(handler(args.argc, args.argv));
	
	// parent

	int status;
	if (waitpid(pid, &status, 0) < 0)
		bailOut(EXIT_FAILURE, "interpreter_forkBuiltIn", "wait");

	if (!WIFEXITED(status)) {
		// bad
	}
	
	return WEXITSTATUS(status);*/
}

error_t interpreter_startBin(char* file, args_t args) {
	int pid = fork();
	if (pid < 0)
		bailOut(EXIT_FAILURE, "interpreter_forkBin", "fork");
	if (pid == 0) {
		if (execv(file, args.argv) < 0)
			bailOut(EXIT_FAILURE, "interpreter_forkBin", "execv");
	}	
	
	// parent

	int status;
	if (waitpid(pid, &status, 0) < 0)
		bailOut(EXIT_FAILURE, "interpreter_forkBin", "wait");

	if (!WIFEXITED(status)) {
		// bad
	}
	
	return WEXITSTATUS(status);
}

int interpreter_do(state_t* state, char* input) {
	if (input == NULL) {
		// this is: EOF
	}

	args_t args = splitter_do(input);

	if (args.argc > 0) {

		program_handler_t handler = interpreter_searchBuiltIn(args.argv[0]);

		error_t result = 0;

		if (handler != NULL) {
			result = interpreter_startBuiltIn(handler, args);
		} else {
			char* file = NULL;
			int error = interpreter_searchBin(args.argv[0], &file);
			if (error == 0) {
				result = interpreter_startBin(file, args);
			} else {
				result = 127; // TODO export to constant
				fprintf(stderr, "%s: command not found: %s\n", PROGRAM_NAME, args.argv[0]);
			}
			free(file);
		}

		state->last = result;
	}

	splitter_free(args);
	
	return -1;
}
