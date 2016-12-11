#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <scheduler.h>
#include <naiveConsole.h>
#include <mutex.h>

mutex_t mutexes[10];
int mutexes_counter;

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
	//ncPrint("get");
	for(int id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			return mutexes[id].taken;	// key already being used
		}
	}
}

void destroyMutexK(int key) {
	//ncPrint("destroy");
	for(int id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			mutexes[id].key = 0;
			mutexes[id].taken = 0;	// key already being used
		}
	}
}

int mutexLockK(int key) {
	//ncPrint("lock");
	uint8_t mutexLock = getMutex(key);
	// ncPrint("ENTRO MIERDA ");
	// ncPrint("MUTEX: ");
    // ncPrintHex(&mutexLock);
    // ncPrint(" valor ");
     // ncPrintDec(mutexLock);
	while(enterCritRegion(&mutexLock)) {
		// ncPrint(" LOCKEADO MAQUILINCE ");
		// ncPrintDec(getCurrentPID());
		yield();
	}
	// ncPrint(" Lo agarro ");
	// ncPrintDec(getCurrentPID());
	// ncPrint(" ");
}

void mutexUnlockK(int key) {
	//ncPrint("UNLOCK");
	for(int id = 0 ; id < 10 ; id++) {
		if(mutexes[id].key == key) {
			mutexes[id].taken = 0;	// key already being used
		}
	// ncPrint("libero");
	// ncPrint("MUTEX: ");
 //    ncPrintHex(mutex);
 //    ncPrint(" ");
 //    ncPrintDec(*mutex);
 //    ncPrint( "liberado por ");
 //    ncPrintDec(getCurrentPID());
 //        ncPrint(" // ");
	}
}