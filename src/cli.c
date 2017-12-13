#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <editline/readline.h>

#include "global.h"
#include "cli.h"
#include "interpreter/interpreter.h"

settings_t* settings;

void cli_init(settings_t* set) {
	settings = set;
}

#define HOSTNAME_MAX_LENGTH 1024
#define CWD_MAX_LENGTH 1024
char* cli_getPrompt(state_t* state) {
	int length = 1;
	const char* username = getlogin();
	if (username == NULL)
		username = "[unknown]";

	char hostname[HOSTNAME_MAX_LENGTH];
	if (gethostname(hostname, HOSTNAME_MAX_LENGTH) != 0)
		bailOut(EXIT_FAILURE, "cli_getPrompt", "gethostname");
	hostname[HOSTNAME_MAX_LENGTH - 1] = '\0'; // for safety

	char* cwd = getcwd(NULL, CWD_MAX_LENGTH);

	char* sep = "$";
	if (getuid() == 0)
		sep = "#";

	length += strlen(username) + 1 + strlen(hostname) + 1 + strlen(cwd) + 1 + 1;

	char* prompt = malloc(length);
	if (prompt == NULL)
		bailOut(EXIT_FAILURE, "cli_getPromp", "malloc");

	(void) strcpy(prompt, username);
	(void) strcat(prompt, "@");
	(void) strcat(prompt, hostname);
	(void) strcat(prompt, ":");
	(void) strcat(prompt, cwd);
	(void) strcat(prompt, sep);
	(void) strcat(prompt, " ");

	return prompt;		
}

char* cli_getLine(state_t* state) {
	char* input = NULL;
	char* prompt = cli_getPrompt(state);

	input = readline(prompt);

	free(prompt);

	if (input == NULL)
		bailOut(EXIT_FAILURE, "cli_getLine", NULL);
	return input;
}

void cli_afterLine(char* input) {
	free(input);
}
