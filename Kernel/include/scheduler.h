#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>
#include <idt.h>
#include <stddef.h>

void * getCurrentEntryPoint();
typedef struct Scheduler {
	Process * currentProcess;
}Scheduler;

Scheduler * newScheduler();

Process * currentProcess = NULL;

pid_t addProcess(void * entryPoint, char * name);
//void removeProcess(Process * process);
int removeProcess(pid_t pid);
void * mem_alloc();
void * userSchedToKernel(uint64_t * rsp);
void * kernelSchedToUser();
void setNextProcess();

#endif