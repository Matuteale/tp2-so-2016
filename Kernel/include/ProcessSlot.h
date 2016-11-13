#ifndef PROCESS_SLOT_H
#define PROCESS_SLOT_H

#include "process.h"

typedef struct ProcessSlot {
	ProcessSlot * next;
	Process * process;	
}ProcessSlot;

ProcessSlot * newProcessSlot(Process * process);

#endif
