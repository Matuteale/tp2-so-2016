#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <scheduler.h>
#include <naiveConsole.h>
#include <mutex.h>
#include <lib.h>

mutex_t mutexes[10];
int mutexes_counter = 0;

int getUsedMutexes(char * names, int * values, int from){
	char * valAux;
	const char * mutexName = "Mutex";
	int size = 0;
  for (int i = 0; i < 10; ++i){
  	if(mutexes[i].key != 0){
  	 	memcpy(&(names[from*11]), mutexName, 6);
  	 	char val[11];
  	 	valAux = itoa(mutexes[i].taken, &(val[0]));
  	 	while(valAux[size] != '\0'){size++;}
  	 	ncPrintDec(size);
  	 	memcpy(&(values[from*11]), valAux, size);
  	 	ncPrint(&(values[from*11]));
  		from++;
  		size = 0;
		}
  }
  return from;
}

void initializeMutexes() {
	for(int id = 0 ; id < 10 ; id++) {
		mutexes[id].key = 0;
	}
}

int createMutexK(int key) {
	int id;
	if(key < 1) {
		return -3;
	}
	for(id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			return -1;	// key already being used
		}
	}
	for(id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == 0) {
			mutexes[id].key = key;
			mutexes[id].taken = 0;
			mutexes_counter++;
			return id;	// returns mutex id
		}
	}
	return -2; // no mutexes available
}

uint8_t getMutex(int key) {
	for(int id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			return mutexes[id].taken;	// key already being used
		}
	}
}

void destroyMutexK(int key) {
	for(int id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			mutexes[id].key = 0;
			mutexes[id].taken = 0;	// key already being used
		}
	}
}

int mutexLockK(int key) {
	uint8_t mutexLock = getMutex(key);
	while(enterCritRegion(&mutexLock)) {
		yield();
	}
}

void mutexUnlockK(int key) {
	for(int id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			mutexes[id].taken = 0;	// key already being used
		}
	}
}