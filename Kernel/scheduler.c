#include <scheduler.h>

Scheduler * newScheduler() {
	current = null;
}

static Scheduler * scheduler = newScheduler();

void * switchUserToKernel(void * esp) {
	Process * process = scheduler->current->process;
	process->userStack = esp;
	return process->kernelStack;
}

void schedule() {
	scheduler->current = current->next;
}

void * switchKernelToUser() {
	scheduler->schedule();
	return scheduler->current->process->userStack;

void * getCurrentEntryPoint() {
	return scheduler->current->process->entryPoint;
}

void * addProcess(Process * process) {
	ProcessSlot * newProcess = new ProcessSlot(process);

	if (scheduler->current == NULL) {
		scheduler->current = newProcess;
		scheduler->current->next = scheduler->current;
	} else {
		ProcessSlot * next = scheduler->current->next;
		scheduler->current->next = newProcess;
		newProcess->next = next;
	}
}

void removeProcess(Process * process) {
	ProcessSlot * prevSlot = scheduler->current;
	ProcessSlot * slotToRemove = scheduler->current->next;

	if (current == NULL) {
		return;
	} else if (prevSlot == slotToRemove && process == current->process) {
		free(scheduler->current);
		scheduler->current = NULL;
		return;
	}

	while(slotToRemove->process != process) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}

	prevSlot->next = slotToRemove->next;
	free(slotToRemove);
}