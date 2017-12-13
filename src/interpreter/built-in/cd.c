#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>

#include "builtin.h"

int cd(int argc, char** argv) {
	char* dir = getenv("HOME");
	if (argc == 2) {
		dir = argv[1];
	}
	if (argc > 2) {
		fprintf(stderr, "cd: too many arguments\n");
		return 1;
	}

	if (chdir(dir) < 0) {
		fprintf(stderr, "cd: chdir: %s", strerror(errno));
		return 2;
	}
	return 0;
}
