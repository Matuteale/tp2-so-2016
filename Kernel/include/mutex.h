#ifndef MUTEX_H
#define MUTEX_H

typedef struct {
	int key;
	int taken;
} mutex_t;

void initializeMutexes();
uint8_t getMutex(int key);
int mutexLockK(int key);
void mutexUnlockK(int key);
void destroyMutexK(int key);
int getUsedMutexes(char * names, char * values, int from);
int createMutexK(int key);
#endif