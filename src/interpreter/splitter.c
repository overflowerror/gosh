#include <stdlib.h>
#include <string.h>

#include "splitter.h"
#include "../global.h"

#define ARGS_ARRAY_RESIZE_STEP 5

args_t splitter_do(char* line) {
	args_t args = (args_t) {0};

	char* tmp = line;
	char* token;
	while ((token = strtok(tmp, " ")) != NULL) {
		tmp = NULL;

		if (args.argc >= args.arraySize) {
			void* tmp = realloc(args.argv, (args.arraySize + ARGS_ARRAY_RESIZE_STEP) * sizeof(char*));
			if (tmp == NULL)
				bailOut(EXIT_FAILURE, "splitter_do", NULL);
			args.argv = tmp;
			args.arraySize += ARGS_ARRAY_RESIZE_STEP;
		}

		void* tmp = malloc(strlen(token) + 1);
		if (tmp == NULL)
				bailOut(EXIT_FAILURE, "splitter_do", NULL);
	
		args.argv[args.argc] = tmp;
		(void) strcpy(args.argv[args.argc], token);
		args.argc++;
	}

	if (args.argc >= args.arraySize) {
		void* tmp = realloc(args.argv, (args.arraySize + ARGS_ARRAY_RESIZE_STEP) * sizeof(char*));
		if (tmp == NULL)
			bailOut(EXIT_FAILURE, "splitter_do", NULL);
		args.argv = tmp;
		args.arraySize += ARGS_ARRAY_RESIZE_STEP;
	}

	args.argv[args.argc] = NULL; // for execv

	return args;
}

void splitter_free(args_t args) {
	for (int i = 0; i < args.argc; i++)
		free(args.argv[i]);

	free(args.argv);
}
