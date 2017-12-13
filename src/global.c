#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "global.h"

bool showShutdownDetails = false;

#define HANDLER_ARRAY_RESIZE_STEP 5

typedef struct {
	const char* name;
	exit_handler_t handler;
} handler_t;

struct cleanUpData {
	int handlerCount;
	int handlerArraySize;
	handler_t* handlers;
} cleanUpData;

int setup() {
	cleanUpData = (struct cleanUpData) {0};

	return 0; // TODO
}

int addExitHandler(exit_handler_t handler, const char* name) {
	if (cleanUpData.handlerCount <= cleanUpData.handlerArraySize) {
		void* tmp = realloc(cleanUpData.handlers, (cleanUpData.handlerArraySize + HANDLER_ARRAY_RESIZE_STEP) * sizeof (handler_t));
		if (tmp == NULL) {
			bailOut(EXIT_SETUP_FAILED, "addExitHandler", name);
		}
		cleanUpData.handlers = tmp;
		cleanUpData.handlerArraySize += HANDLER_ARRAY_RESIZE_STEP;
	}
	cleanUpData.handlers[cleanUpData.handlerCount] = (handler_t) {
		.name = name,
		.handler = handler
	};
	cleanUpData.handlerCount++;

	return 0; // TODO: we need something better here
}

void cleanUp() {
	for (int i = 0; i < cleanUpData.handlerCount; i++) {
		int code = cleanUpData.handlers[i].handler();
		if (showShutdownDetails) {
			if (code != 0) {
				fprintf(stderr, "exit handler %s terminated with error (%d)\n", cleanUpData.handlers[i].name, code);
			} else {
				fprintf(stderr, "exit handler %s terminated successfully\n", cleanUpData.handlers[i].name);
			}
		}
	}
	fprintf(stderr, "freeing exit handler array (%d)\n", cleanUpData.handlerCount);
}


void bailOut(int exitcode, const char* module, const char* message) {
	if (module != NULL) {
		// output is not split into smaller quantities to 
		// ensure error messages of child and parent 
		// don't get mixed
		if (message == NULL) {
			if (errno == 0) {
				(void) fprintf(stderr, "%s: %s\n", PROGRAM_NAME, module);
			} else {
				(void) fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, module, strerror(errno));
			}
		} else {
			if (errno == 0) {
				(void) fprintf(stderr, "%s: %s (%s)\n", PROGRAM_NAME, module, message);
			} else {
				(void) fprintf(stderr, "%s: %s (%s): %s\n", PROGRAM_NAME, module, message, strerror(errno));
			}
		}
	}

	fprintf(stderr, "cleaning up...");
	cleanUp();

	fprintf(stderr, "exiting with error code %d", exitcode);
	exit(exitcode);
}
