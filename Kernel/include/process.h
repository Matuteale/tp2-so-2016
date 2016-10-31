#ifndef PROCESS_H
#define PROCESS_H

struct Process {
	void * userStack;
	void * kernelStack;
	void * entryPoint;
};

static void * toStackAddress(void * page);
static void * fillStackFrame(void * entryPoint, void * userStack);