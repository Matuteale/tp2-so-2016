#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

void * getCurrentEntryPoint();
typedef struct Scheduler {
	ProcessSlot * currentProcess;
}Scheduler;

Scheduler * newScheduler();

void * schedule(void * esp);
void addProcess(Process * process);
void removeProcess(Process * process);

#endif