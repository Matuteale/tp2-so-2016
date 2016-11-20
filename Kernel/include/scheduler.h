#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>

void * getCurrentEntryPoint();
typedef struct Scheduler {
	Process * currentProcess;
}Scheduler;

Scheduler * newScheduler();

void * schedule(void * esp);
pid_t addProcess(void * entryPoint, char * name);
//void removeProcess(Process * process);
int removeProcess(pid_t pid);
void * mem_alloc();

#endif