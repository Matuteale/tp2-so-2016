#include <process.h>
#include <stdio.h>
#include <stdint.h>
#include <memManager.h>
#define NO_MEMORY 0

typedef int (*EntryPoint)();

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

	static ProcessTable * PTable;
	
	Process * initializeProcess(void * entryPoint) {
		PTable->table[PTable->counter] = newProcess(entryPoint);
		Process * aux = PTable->table[PTable->counter];
		PTable->counter++;
		return aux;
	}


	void initializeProcessTable(void * entryPoint) {
		PTable->counter = 0;
		addProcess(initializeProcess(entryPoint));
				((EntryPoint)entryPoint)();

	} 



	Process * newProcess(void * entryPoint) {
		Process * process;
		process->entryPoint = entryPoint;
		process->nOfPages = 0;
		process->stack = fillStackFrame(entryPoint, process->stack);
		return process;
	}

	void * toStackAddress(void * page) {
		return (uint8_t*)page + PAGE_SIZE - 0x10;
	}

	void * fillStackFrame(void * entryPoint, void * stack) {
		StackFrame * frame = (StackFrame*)stack - 1;
		frame->gs =		0x001;
		frame->fs =		0x002;
		frame->r15 =	0x003;
		frame->r14 =	0x004;
		frame->r13 =	0x005;
		frame->r12 =	0x006;
		frame->r11 =	0x007;
		frame->r10 =	0x008;
		frame->r9 =		0x009;
		frame->r8 =		0x00A;
		frame->rsi =	0x00B;
		frame->rdi =	0x00C;
		frame->rbp =	0x00D;
		frame->rdx =	0x00E;
		frame->rcx =	0x00F;
		frame->rbx =	0x010;
		frame->rax =	0x011;
		frame->rip =	(uint64_t)entryPoint;
		frame->cs =		0x008;
		frame->eflags = 0x202;
		frame->rsp =	(uint64_t)&(frame->base);
		frame->ss = 	0x000;
		frame->base =	0x000;

	return frame;
	}

		static void * auxMemAlloc(Process * process, uint64_t size, uint64_t pageNum) {
		if(process->nOfPages <= pageNum) {
			pageManager(POP_PAGE, (process->memStack) + pageNum*PAGE_SIZE);
			void * result = process->memStack;
			process->memStackOffset[pageNum] = size;
			process->nOfPages += 1;
			return result;	
		}
		if((PAGE_SIZE - process->memStackOffset[0]) <= size){
			void * result = process->memStack+ PAGE_SIZE*pageNum + process->memStackOffset[pageNum];
			process->memStackOffset[pageNum] += size;
			return result;
		}
		else {
			return auxMemAlloc(process, size, pageNum+1);
		}
	}

	static pid_t getPID(Process * process) {
		return process->PID;
	}

	static pid_t getParentPID(Process * process) {
		return process->ParentPID;
	}

	void * OSalloc(Process * process) {
		pageManager(POP_PAGE, process->memStack + PAGE_SIZE*process->nOfPages);
		void * result = process->memStack + PAGE_SIZE*process->nOfPages;
		process->nOfPages++;
		return result;
	}

	static void * malloc(Process * process, uint64_t size) {
		if(size > PAGE_SIZE) {
			return NO_MEMORY;
		}
		if(process->nOfPages == 0) {
			pageManager(POP_PAGE, process->memStack);
			void * result = process->memStack;
			process->memStackOffset[0] = size;
			process->nOfPages += 1;
			return result;
		}
		if((PAGE_SIZE - process->memStackOffset[0]) <= size){
			void * result = process->memStack + process->memStackOffset[0];
			process->memStackOffset[0] += size;
			return result;
		}
		else {
			return auxMemAlloc(process, size, 1);
		}
	}