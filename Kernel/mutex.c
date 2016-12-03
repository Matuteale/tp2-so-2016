#include <stdint.h>
#include <pthread.h>

int mutexInit(phtread_attr_t * mutexLock) {
	return pthread_attr_init(mutexLock);
}

int mutexDestroy(pthread_attr_t * mutexLock) {
	return pthread_attr_destroy(mutexLock);
}

int mutexLock(uint8_t * mutexLock) {
	while(enterCritRegion(mutexLock)) {
		yield();
	}
}

void mutexUnlock(uint8_t * mutex) {
	*mutex = 0;
}