#ifndef PROCESS_SLOT_H
#define PROCESS_SLOT_H

#include <process.h>

typedef struct ProcessSlot {
	struct ProcessSlot * next;
	struct Process * process;	
}ProcessSlot;

ProcessSlot * newProcessSlot(Process * process);

#endif
