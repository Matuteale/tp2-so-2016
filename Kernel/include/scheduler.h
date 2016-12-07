#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>
#include <idt.h>
#include <sleep.h>

void * getCurrentEntryPoint();
typedef struct Scheduler {
	Process * currentProcess;
}Scheduler;

Scheduler * newScheduler();

pid_t addProcess(void * entryPoint, char * name, int isBackground);
//void removeProcess(Process * process);
int removeProcess(pid_t pid);
void * mem_alloc();
void * userSchedToKernel(uint64_t * rsp);
void * kernelSchedToUser();
void setNextProcess();
Process * getProcessArray();
pid_t getCurrentPID();
void changeProcessState(pid_t pid, ProcessState state);
Process * getCurrentProcess();
void nullProcess();
void setName(Process * process, char * name);

#endif