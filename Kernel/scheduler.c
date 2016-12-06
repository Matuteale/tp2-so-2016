#include <scheduler.h>
#include <naiveConsole.h>
#include <stddef.h>

#define STACKKKK 4096

int last_pid_given = 0;

int freeProcesses = 16;

Process process[16];

Process * currentProcess = NULL;

Process * freeProcess = NULL;

Process * nilProcess = NULL;

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

		} while(current->state != RUNNING && current->state != READY);
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

void nullProcess()
{
	while(1){
		ncPrint("A");
	};
}

void changeProcessState(pid_t pid, ProcessState state) {
	int i;
	for(i = 0; i<16; i++) {
		if(process[i].PID == pid) {
			process[i].state = state;
		}
	}
}


void initializeScheduler() {

	addProcess(nullProcess, "Null", 1);

	addProcess(codeModuleAddress, "Shell", 0);

	inizialized = 1;

	scheduleNow();
}

void linkProcessStructures() {

	for(int i = 0 ; i < 15 ; i++) {

		process[i].next = &process[i+1];

	}

	freeProcess = &process[0];

}


pid_t addProcess(void * entry_point, char * name, int isBackground) {
	//ncPrint("EntryPoint: ");
	//ncPrintHex(entry_point);
	//ncNewline();

	Process * new_process = &process[counter++];

	//new_process->stack = stackkkk;
  new_process->stack = mem_alloc();
	new_process->stack = fillStackFrame(entry_point, (void *) new_process->stack + STACKKKK);

	if(freeProcesses == 0) {
		return -1;
	}

	//new_process = freeProcess;

	//freeProcess = freeProcess->next_freeProcess;

	new_process->PID = getNewPid();
	new_process->entryPoint = entry_point;
	setName(new_process, name);

	if(currentProcess == NULL) {

		ncPrint("Agrego el Null process");
		ncNewline();

		nilProcess = new_process;

		currentProcess = new_process;

		new_process->next = new_process;

		new_process->state = DEAD;

	} else {

		ncPrint("Agrego el nuevo process");

		new_process->next = currentProcess->next;

		currentProcess->next = new_process;

		if(!isBackground){
			if(nilProcess->PID == currentProcess->PID){
				currentProcess->state = DEAD;
			}else{
				currentProcess->state = READY;
			}
			new_process->state = RUNNING;
			new_process->foreground = 1;
		}else{
			new_process->state = READY;
			new_process->foreground = 0;
		}

	}

	--freeProcesses;

	// ncNewline();
	// ncPrint("EntryPoint: ");
	// ncPrintHex(new_process->entryPoint);
	// ncNewline();
	// ncPrint("stack: ");
	// ncPrintHex(new_process->stack);
	// ncNewline();
	// ncPrint("next: ");
	// ncPrintDec(new_process->next->PID);
	// ncNewline();
	// ncPrint("PID: ");
	// ncPrintDec(new_process->PID);
	// ncNewline();

	clearscreen();

	return new_process->PID;

}

Process * getCurrentProcess()
{
	return currentProcess;
}

int removeProcess(pid_t pid) {
	if(pid == 1) return; //NULL process
	Process * process = currentProcess;
	Process * processAux = NULL;
	while(process->PID != pid){
		processAux = process;
		process = process->next;
	}
	if(processAux == NULL){
		processAux = process;
		while(process->PID != processAux->next->PID){
			processAux = processAux->next;
		}
	}
	processAux->next = process->next;
	if(pid == currentProcess->PID){
		//currentProcess->state = INACTIVE;
		if(currentProcess->next->PID == 1){
			//currentProcess->next->next->state = ACTIVE;
		}else{
			//currentProcess->next->state = ACTIVE;
		}
		currentProcess = currentProcess->next;
		scheduleNow();
	}
	//clearscreen();
}

pid_t getCurrentPID() {
	return currentProcess->PID;
}

void * mem_alloc() {
	return alloc();
}

void setName(Process * process, char * name){
	memset(process->name, 0, 24);
	int i = 0;
	while(name[i++] != 0);
  memcpy(process->name, name, i+1);
}
