#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <ProcessSlot.h>

void * getCurrentEntryPoint();
typedef struct Scheduler {
	ProcessSlot * currentProcess;
}Scheduler;

Scheduler * newScheduler();

void * schedule(void * esp);
void addProcess(Process * process);
void removeProcess(Process * process);
void * mem_alloc();

#endif