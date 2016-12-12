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
	DYING,
	SLEEPING,
	NIL

} ProcessState;

typedef struct Process {
	char name[24];
	void * stack;
	int foreground;
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
void * toStackAddress(void * page);
void * OSalloc(Process * process);
static pid_t getPID(Process * process);
static pid_t getParentPID(Process * process);

#endif