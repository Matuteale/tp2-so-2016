#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <naiveConsole.h>


int mutexLockK(uint8_t * mutexLock) {
	// ncPrint("ENTRO MIERDA");
	while(enterCritRegion(mutexLock)) {
		// ncPrint("LOCKEADO MAQUILINCE");
		yield();
	}
	// ncPrint("ENTRO");
	// ncPrint("ENTRO");
}

void mutexUnlockK(uint8_t * mutex) {
	*mutex = 0;
}