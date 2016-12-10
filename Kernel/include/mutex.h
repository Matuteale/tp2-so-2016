#ifndef MUTEX_H
#define MUTEX_H

typedef struct {
	int key;
	int taken;
} mutex_t;

void initializeMutexes(); 
uint8_t getMutex(int key);
int mutexLock(uint8_t * mutexLock);
void mutexUnlock(uint8_t * mutex);
void destroyMutexK(int key);
#endif