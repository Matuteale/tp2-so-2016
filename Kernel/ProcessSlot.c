#include "ProcessSlot.h"

ProcessSlot * ProcessSlot(Process * process) {
	ProcessSlot * slot;
	slot->process = process;
	return slot;
}

