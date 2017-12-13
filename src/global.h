#ifndef GLOBAL_H
#define GLOBAL_H

#define PROGRAM_NAME "gosh"
#define VERSION "0.1"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_SETUP_FAILED 10

#define EXIT_IMPOSSIBLE_STATE 255

#include <stdbool.h>

typedef struct {
	int verbose;
} settings_t;

typedef int (*exit_handler_t)();

extern bool showShutdownDetails;

/**
 * @return 0 on success, 1 else
 */
int setup();

int addExitHandler(exit_handler_t, const char*);

void cleanUp();

void bailOut(int, const char*, const char*);

#endif
