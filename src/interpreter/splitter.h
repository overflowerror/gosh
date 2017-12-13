#ifndef SPLITTER_H
#define SPLITTER_H

typedef struct {
	int argc;
	int arraySize;
	char** argv;
} args_t;

args_t splitter_do(char*);

void splitter_free(args_t);

#endif
