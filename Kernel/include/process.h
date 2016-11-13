#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process {
	void * stack;
	void * entryPoint;
} Process;

static void * toStackAddress(void * page);
static void * fillStackFrame(void * entryPoint, void * userStack);

#endif