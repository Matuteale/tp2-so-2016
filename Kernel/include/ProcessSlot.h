#ifndef PROCESS_SLOT_H
#define PROCESS_SLOT_H

#include "Process.h"

struct ProcessSlot {
	ProcessSlot * next;
	Process * process;	
};

ProcessSlot * ProcessSlot(Process * process);

#endif
