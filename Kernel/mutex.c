#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <naiveConsole.h>


int mutexLockK(uint8_t * mutexLock) {
	while(enterCritRegion(mutexLock)) {
		yield();
	}
}

void mutexUnlockK(uint8_t * mutex) {
	*mutex = 0;
}