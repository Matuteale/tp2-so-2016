/*#include <scheduler.h>
#include <ProcessSlot.h>

Scheduler * newScheduler() {
	Scheduler * scheduler;
	scheduler->currentProcess; //Allocate Mem for circular queue
	return scheduler;
}

static Scheduler * scheduler;

void initializeScheduler() {
	scheduler = newScheduler();
}

void * switchUserToKernel(void * esp) {
	Process * process = scheduler->currentProcess->process;
	process->userStack = esp;
	return process->kernelStack;
}

void schedule() {
	scheduler->currentProcess = currentProcess->next;
}

void * switchKernelToUser() {
	schedule();
	return scheduler->currentProcess->process->userStack;

void * getCurrentProcessEntryPoint() {
	return scheduler->currentProcess->process->entryPoint;
}

void * addProcess(Process * process) {
	ProcessSlot * newProcess = new ProcessSlot(process);

	if (!scheduler->currentProcess) {
		scheduler->currentProcess = newProcess;
		scheduler->currentProcess->next = scheduler->currentProcess;
	} else {
		ProcessSlot * next = scheduler->currentProcess<->next;
		scheduler->currentProcess->next = newProcess;
		newProcess->next = next;
	}
}

void removeProcess(Process * process) {
	ProcessSlot * prevSlot = scheduler->currentProcess;
	ProcessSlot * slotToRemove = scheduler->currentProcess->next;

	if (!scheduler->currentProcess) {
		return;
	} else if (prevSlot == slotToRemove && process == currentProcess->process) {
		free(scheduler->currentProcess);
		scheduler->currentProcess = NULL;
		return;
	}
	//If entered here there is more than 1 process in the scheduler
	while(slotToRemove->process != process) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}
	prevSlot->next = slotToRemove->next;
	free(slotToRemove);
}*/