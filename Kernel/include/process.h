#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include <memManager.h>
#include <sys/types.h>

typedef struct Process {
	void * stack;
	void * memStack [STACK_SIZE];
	uint64_t nOfPages;
	uint64_t memStackOffset[STACK_SIZE];
	void * entryPoint;
	pid_t PID;
	pid_t ParentPID;

} Process;

static void * toStackAddress(void * page);
static void * fillStackFrame(void * entryPoint, void * stack);
static void * auxMemAlloc(Process * process, uint64_t size, uint64_t pageNum);
static void * malloc(Process * process, uint64_t size);
static pid_t getPID();
static pid_t getParentPID();


#endif