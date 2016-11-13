#include <scheduler.h>

Scheduler * newScheduler() {
	Scheduler * scheduler;
	scheduler->currentProcess = null;
	return scheduler;
}

static Scheduler * scheduler = newScheduler();

void * switchUserToKernel(void * esp) {
	Process * process = scheduler->currentProcess->process;
	process->userStack = esp;
	return process->kernelStack;
}

void schedule() {
	scheduler->currentProcess = currentProcess->next;
}

void * switchKernelToUser() {
	scheduler->schedule();
	return scheduler->currentProcess->process->userStack;

void * getCurrentProcessEntryPoint() {
	return scheduler->currentProcess->process->entryPoint;
}

void * addProcess(Process * process) {
	ProcessSlot * newProcess = new ProcessSlot(process);

	if (scheduler->currentProcess == NULL) {
		scheduler->currentProcess = newProcess;
		scheduler->currentProcess->next = scheduler->currentProcess;
	} else {
		ProcessSlot * next = scheduler->currentProcess->next;
		scheduler->currentProcess->next = newProcess;
		newProcess->next = next;
	}
}

void removeProcess(Process * process) {
	ProcessSlot * prevSlot = scheduler->currentProcess;
	ProcessSlot * slotToRemove = scheduler->currentProcess->next;

	if (currentProcess == NULL) {
		return;
	} else if (prevSlot == slotToRemove && process == currentProcess->process) {
		free(scheduler->currentProcess);
		scheduler->currentProcess = NULL;
		return;
	}

	while(slotToRemove->process != process) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}

	prevSlot->next = slotToRemove->next;
	free(slotToRemove);
}