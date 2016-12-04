#include <scheduler.h>
#include <naiveConsole.h>
#include <stddef.h>

#define STACKKKK 4096

int last_pid_given = 0;

int freeProcesses = 16;

Process process[16];

Process * currentProcess = NULL;

Process * freeProcess = NULL;

int inizialized = 0;

char stackkkk[STACKKKK];

int counter = 0;

extern void * kernelStack;

static void * const codeModuleAddress = (void*)0x400000;

typedef struct StackFrame {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
}StackFrame;


void * fillStackFrame(void * entryPoint, void * userStack) {

	StackFrame * stackFrame = (StackFrame *) userStack - 1;

	stackFrame->gs = 0x01;
	stackFrame->fs = 0x02;
	stackFrame->r15 = 0x03;
	stackFrame->r14 = 0x04;
	stackFrame->r13 = 0x05;
	stackFrame->r12 = 0x06;
	stackFrame->r11 = 0x07;
	stackFrame->r10 = 0x08;
	stackFrame->r9 = 0x09;
	stackFrame->r8 = 0x0A;
	stackFrame->rsi = 0x0B;
	stackFrame->rdi = 0x0C;
	stackFrame->rbp = 0x0D;
	stackFrame->rdx = 0x0E;
	stackFrame->rcx = 0x0F;
	stackFrame->rbx = 0x010;
	stackFrame->rax = 0x011;
	stackFrame->rip = (uint64_t) entryPoint;
	stackFrame->cs = 0x008;
	stackFrame->eflags = 0x202;
	stackFrame->rsp = (uint64_t) userStack;
	stackFrame->ss = 0x000;
	stackFrame->base = 0x000;

	return stackFrame;
}

void * userSchedToKernel(uint64_t * rsp){

	if (currentProcess != 0) { //NULL
		currentProcess->stack = rsp;
	}
	return kernelStack;
}

void setNextProcess(){
	Process * current = currentProcess;
	if(current != NULL && current->next != NULL){
		do {
			current = current->next;

		} while(current->state != ACTIVE);
		currentProcess = current;
	}
}

void * kernelSchedToUser(){
	if(currentProcess == NULL){
		return kernelStack;
	}
	return currentProcess->stack;
}

pid_t getNewPid() {

	return ++last_pid_given;

}

void printA() {

	while(1) {
		ncPrint("A");
	}

}

void printB() {

	while(1) {

	}

}


void initializeScheduler() {

	addProcess(0, "Null");

	addProcess(codeModuleAddress, "Shell");

	inizialized = 1;

	scheduleNow();
}

void linkProcessStructures() {

	for(int i = 0 ; i < 15 ; i++) {

		process[i].next = &process[i+1];

	}

	freeProcess = &process[0];

}


pid_t addProcess(void * entry_point, char * name) {
	//ncPrint("EntryPoint: ");
	//ncPrintHex(entry_point);
	//ncNewline();

	Process * new_process = &process[counter++];

	//new_process->stack = stackkkk;

	new_process->stack = fillStackFrame(entry_point, (void *) new_process->stack + STACKKKK);

	if(freeProcesses == 0) {
		return -1;
	}

	//new_process = freeProcess;

	//freeProcess = freeProcess->next_freeProcess;

	new_process->PID = getNewPid();
	new_process->entryPoint = entry_point;
	memset(new_process->name, 0, 24);
	int i = 0;
	while(name[i++] != 0);
  memcpy(new_process->name, name, i+1);

	// copy name

	//new_process->stack = fillStackFrame(entry_point, (char *) &currentProcess->stack + STACK_SIZE);

	if(currentProcess == NULL) {

		ncPrint("Agrego el Null process");
		ncNewline();

		currentProcess = new_process;

		new_process->next = new_process;

		new_process->state = INACTIVE;

	} else {

		ncPrint("Agrego el nuevo process");

		new_process->next = currentProcess->next;

		currentProcess->next = new_process;

		currentProcess->state = INACTIVE;

		new_process->state = ACTIVE;

		clearscreen();

	}

	--freeProcesses;
	ncNewline();
	ncPrint("EntryPoint: ");
	ncPrintHex(new_process->entryPoint);
	ncNewline();
	ncPrint("stack: ");
	ncPrintHex(new_process->stack);
	ncNewline();
	ncPrint("next: ");
	ncPrintDec(new_process->next->PID);
	ncNewline();
	ncPrint("PID: ");
	ncPrintDec(new_process->PID);
	ncNewline();

	return new_process->PID;

}

Process * getCurrentProcess()
{
	return currentProcess;
}

int removeProcess(pid_t pid) {


}

void * mem_alloc() {
	return alloc();
}

/*
Scheduler * scheduler;
int first_switch = 1;
int last_pid_given = 0;
int freeProcesses = 16;
ProcessTable * processes;
void * schedule(void * esp) {
	if(first_switch == 1) {
		first_switch = 0;
		initializeScheduler();
	}
	Process * process;
	//while(1);
	process = scheduler->currentProcess->process;
	process->stack = esp;
	scheduler->currentProcess = scheduler->currentProcess->next; //Cambio de proceso
	return scheduler->currentProcess->process->stack; //Devuelvo el stack del nuevo proceso
}
Scheduler * newScheduler() {
	Scheduler * scheduler;
	pageManager(POP_PAGE, scheduler->currentProcess); //Allocate Mem for circular queue
	return scheduler;
}
void dummy() {
	while(1);
}
void * getCurrentProcessEntryPoint() {
	return scheduler->currentProcess->process->entryPoint;
}
void initializeScheduler() {
	scheduler = newScheduler();
	addProcess(&dummy);
}
void addProcess(void * entryPoint) {
	Process * process = newProcess(entryPoint);
	ProcessSlot * newProcess = newProcessSlot(process);
	if (!scheduler->currentProcess) {
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
	if (!scheduler->currentProcess) {
		return;
	} else if (prevSlot == slotToRemove && process == scheduler->currentProcess->process) {
		//free(scheduler->currentProcess);
		return;
	}
	//If entered here there is more than 1 process in the scheduler
	while(slotToRemove->process != process) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}
	prevSlot->next = slotToRemove->next;
	//free(slotToRemove);
}
void * mem_alloc() {
	return OSalloc(scheduler->currentProcess->process);
}
/*
Scheduler * newScheduler() {
	Scheduler * scheduler;
	scheduler->currentProcess; //Allocate Mem for circular queue
	return scheduler;
}
static Scheduler * scheduler;
void initializeScheduler() {
	scheduler = newScheduler();
}
void * schedule(void * esp) {
	Process * process = scheduler->currentProcess->process;
	process->stack = esp;
	scheduler->currentProcess = currentProcess->next; //Cambio de proceso
	return scheduler->currentProcess->process->stack; //Devuelvo el stack del nuevo proceso
}
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