#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

void * switchUserToKernel(void * esp);
void * switchKernelToUser();
void * getCurrentEntryPoint();
typedef struct Scheduler {
	ProcessSlot * currentProcess;
}Scheduler;

Scheduler * newScheduler();

void schedule();
void addProcess(Process * process);
void removeProcess(Process * process);