#ifndef PROCESS_H
#define PROCESS_H
#define MAX_PROCESSES 16
#include <stdint.h>
#include <memManager.h>
#include <sys/types.h>

typedef enum {

	RUNNING = 0,
	BLOCKED,
	READY,
	DEAD

} ProcessState;

typedef struct Process {
	void * stack;
	void * stackPointer;
	uint64_t nOfPages;
	void * entryPoint;
	pid_t PID;
	pid_t ParentPID;
	ProcessState state;
	struct Process * next;
} Process;

typedef struct ProcessTable {
	Process * table[MAX_PROCESSES];
	int counter;
} ProcessTable;

ProcessTable * newProcessTable();
Process * newProcess();
void initializeProcessTable(void * entryPoint);
void intializeProcess(void * entryPoint);
static void * toStackAddress(void * page);
static void * fillStackFrame(void * entryPoint, void * stack);
static void * auxMemAlloc(Process * process, uint64_t size, uint64_t pageNum);
static void * malloc(Process * process, uint64_t size);
void * OSalloc(Process * process);
static pid_t getPID(Process * process);
static pid_t getParentPID(Process * process);


#endif