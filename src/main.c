#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <unistd.h>
#include <getopt.h>

#include "global.h"
#include "cli.h"
#include "interpreter/interpreter.h"

void version() {
	printf("%s Version %s\n", PROGRAM_NAME, VERSION);

	cleanUp();
	exit(EXIT_SUCCESS);
}

void parseArguments(int argc, char** argv, settings_t* settings) {
	const char* options = "v";
	const struct option long_options[] = {
		{"version",	no_argument,	0,	'v'}
	}; 

	int opt;
	while (true) {
		int option_index = 0;
		opt = getopt_long(argc, argv, options, long_options, &option_index);

		if (opt == -1)
			break;

		switch(opt) {
			case 0: // long option
				if (option_index == 0) // version
					version();
				break;
			case 'v':
				version();
				break;
			default:
				assert(false);
				exit(EXIT_IMPOSSIBLE_STATE);
		}
	}

	if (optind < argc) {
		// non-option arguments
	}
}

int main(int argc, char** argv) {
	setup();

	settings_t settings = {0};

	parseArguments(argc, argv, &settings);

	cli_init(&settings);
	interpreter_init(&settings);
	
	char* input;
	int result;
	state_t state = interpreter_initState();
	while (true) {
		input = cli_getLine(&state);
		result = interpreter_do(&state, input);
		cli_afterLine(input);

		if (result >= 0 && result < 256) {
			break; // result is exit code
		}
	}
	
	return result;
}
