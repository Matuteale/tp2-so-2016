#include <scheduler.h>
#include <naiveConsole.h>
#include <stddef.h>
#include <idt.h>


#define STACKKKK 4096

int last_pid_given = 0;

int freeProcesses = 16;

int schedulerPaused = 1;

Process process[16];

Process * currentProcess = NULL;

// Process * freeProcess = NULL;

Process * nilProcess = NULL;

Process * shellProcess = NULL;

int inizialized = 0;

char stackkkk[STACKKKK];

int counter = 0;

int waitingProcess[16];
int waitingMilis[16];
int timertickFlag = 7000;

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

int checkIfSchedulerPaused() {
	return schedulerPaused;
}

void pauseScheduler() {
	schedulerPaused = 0;
}

void unpauseScheduler() {
	schedulerPaused = 1;
}

void * userSchedToKernel(uint64_t * rsp){

	if (currentProcess != 0) { //NULL
		currentProcess->stack = rsp;
	}
	// ncPrint("PID: ");
	// ncPrintDec(currentProcess->PID);
	return kernelStack;
}

void setNextProcess(){
	Process * current = currentProcess;
	Process * auxProcess = NULL;
	if(current != NULL && current->next != NULL){
		do {
			auxProcess = current;
			current = current->next;
			if(current->state == DYING){
				Process * aux = current;
				current = current->next;
				freeProcess(aux->PID);
			}
			wakeOrContinueSleep(current);
		} while(current->state != RUNNING && current->state != READY);
		if(currentProcess->state != DYING && currentProcess->state != NIL){
			currentProcess->state = READY;
		}
		currentProcess = current;
		currentProcess->state = RUNNING;
	}
}

void wakeOrContinueSleep(Process * process){
	if(process->state == SLEEPING){
		int i = 0;
		for(; i < 16; i++){
			if(waitingProcess[i] = process->PID){
				break;
			}
		}
		timertickFlag--;
		if(timertickFlag == 0){
			waitingMilis[i] = waitingMilis[i] - 1;
			timertickFlag = 7000;
		}
		if(waitingMilis[i] <= 0){
			changeProcessState(waitingProcess[i], READY);
			waitingProcess[i] = -1;
		}
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
	};
}

int * getWaitingProcess(){
	return waitingProcess;
}

int * getWaitingMilis(){
	return waitingMilis;
}


void changeProcessState(pid_t pid, ProcessState state) {
	for(int i = 0; i<16; i++) {
 		if(process[i].PID == pid) {
 			process[i].state = state;
 		}
 	}
	Process * auxProcess = currentProcess;
	while(currentProcess->PID != pid) {
		auxProcess = auxProcess->next;
	}
	auxProcess->state = state;
}


void initializeScheduler() {

	fillWaitings();

	addProcess(nullProcess, "Null", 1);

	addProcess(codeModuleAddress, "Shell", 0);

	inizialized = 1;

	scheduleNow();
}

void fillWaitings(){
	for(int i = 0; i < 16; i++){
		waitingProcess[i] = -1;
		waitingMilis[i] = -1;
	}
}



pid_t addProcess(void * entry_point, char * name, int isBackground) {
	//ncPrint("EntryPoint: ");
	//ncPrintHex(entry_point);
	//ncNewline();

	Process * new_process = alloc();

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

		nilProcess = new_process;

		currentProcess = new_process;

		new_process->next = new_process;

		new_process->state = NIL;

		new_process->foreground = 0;

	} else {

		// ncPrint("Agrego el nuevo process");
		// ncNewline();
		if(currentProcess->PID == nilProcess->PID){
			new_process->next = currentProcess;
			currentProcess->next = new_process;
			shellProcess = new_process;
		}else{
			new_process->next = currentProcess->next;
			currentProcess->next = new_process;
		}

		if(!isBackground){
			// ncPrint("nobackground");
			// ncNewline();
			currentProcess->foreground = 0;
			new_process->state = READY;
			new_process->foreground = 1;
		}else{
			// ncPrint("isbackground");
			// ncNewline();
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
	 ncPrint("Agrego en sched PID: ");
	 ncPrintDec(new_process->PID);
	 ncNewline();

	//clearscreen();

	return new_process->PID;

}

void freeProcess(pid_t pid) {
	ncPrint("saco el: ");
	ncPrintDec(pid);
	ncNewline();
	Process * prevProcess = currentProcess;
	Process * processToRemove;
	while(prevProcess->next->PID != pid) {
		ncPrint("Paso por: ");
	ncPrintDec(prevProcess->next->PID);
	ncNewline();

		prevProcess = prevProcess->next;
		ncPrint("AYA");
	}
	processToRemove = prevProcess->next;
	prevProcess->next = processToRemove->next;
	freeMem(processToRemove);
	freeProcesses++;
}

Process * getCurrentProcess()
{
	return currentProcess;
}

int removeProcess(pid_t pid) {
	// resetTarget();
	ncPrint("Process to remove ");
	ncPrintDec(pid);
	if(pid == nilProcess->PID || pid == shellProcess->PID) return -1;
	Process * process = currentProcess;
	Process * processAux = NULL;
	while(process->PID != pid){ //Process=processToRemove
		process = process->next;
	}
	if(pid == process->PID){
		process->state = DYING;
		process->foreground = 0;
		processAux = process;
		if(shellProcess != NULL){
			process = shellProcess;
		}else{
			do{
				process = process->next;
			} while(process->PID != processAux->PID && process->state != READY);
		}
		if(process->state == READY){
			process->foreground = 1;
		}
	}else{
		process->state = DYING;
		process->foreground = 0;
	}
	//clearscreen();
	return processAux->PID;
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
