#include "ProcessSlot.h"

ProcessSlot * newProcessSlot(Process * process) {
	ProcessSlot * slot;
	slot->process = process;
	return slot;
}

