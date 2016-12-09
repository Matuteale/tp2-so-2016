#ifndef MUTEX_H
#define MUTEX_H

typedef struct {
	int key;
	int taken;
} mutex_t;

int mutexLock(uint8_t * mutexLock);
void mutexUnlock(uint8_t * mutex);

#endif