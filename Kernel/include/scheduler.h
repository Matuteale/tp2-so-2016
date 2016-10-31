#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"

void * switchUserToKernel(void * esp);
void * switchKernelToUser();
void * getCurrentEntryPoint();
struct Scheduler {
	ProcessSlot * currentProcess;
};

Scheduler * newScheduler();

void schedule();
void addProcess(Process * process);
void removeProcess(Process * process);