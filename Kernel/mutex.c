#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



int mutexLock(uint8_t mutexLock) {
	while(enterCritRegion(&mutexLock)) {
		yield();
	}
}

void mutexUnlock(uint8_t mutex) {
	mutex = 0;
}