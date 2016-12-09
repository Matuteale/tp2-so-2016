#include <stdint.h>
#include <scheduler.h>
#include <process.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <naiveConsole.h>
#include <stdlib.h>
#include <condVar.h>
#include <mutex.h>

cond_t condVars[30];
int condVars_counter;

void initalizeCVs() {
    for(int id = 0 ; id < 30 ; id++) {
        initCondVarK(&condVars[id]);
        condVars[id].key = 0;
    }
}

void initCondVarK(cond_t * condVar) {
    condVar->index = 0;
	condVar->size = 0;
}

void createCondVarsK(int key) {
    int id;
    if(key < 1) {
        return -3;
    }
    for(id = 0 ; id < 30 ; id++) {
        if(condVars[id].key == key) {
            return -1;  // key already being used
        }
    }
    for(id = 0 ; id < 30 ; id++) {
        if(condVars[id].key == 0) {
            condVars[id].key = key;
            condVars_counter++;
            return id;  // returns mutex id
        }
    }
    return -2; // no mutexes available
}

cond_t * getCV(int key) {
    for(int id = 0 ; id < 30 ; id++) {
        if(condVars[id].key == key) {
            return &(condVars[id]);   // key already being used
        }
    }
}

void waitCondVarK(int condVar, int mutex){
    cond_t * cv = getCV(condVar);
    // mutex_t m = getMutex(mutex);
    // ncPrint("ESPERA   -   ");
    // ncPrintHex(condVar);
    pauseScheduler();
    cv->mutex = mutex;
    addToCondVarQueueK(cv,getCurrentPID());
    changeProcessState(getCurrentPID(),1); //blocked
    mutexUnlockK(mutex);
    // ncPrint("MUTEX: ");
    // ncPrintDec(mutex);
    unpauseScheduler();
    // ncPrint(" VA A YIELDEAR");
    yield();
    // ncPrint(" yieldio");
    mutexLockK(mutex);
}

void signalCondVarK(int condVar) {
	int pid = removeFromCondVarQueue(condVar);
    if(pid != -1) {
    // ncPrint("\nAWAKEN MY MASTES\n");
    changeProcessState(pid,0);	//ready
    }
}




void addToCondVarQueueK(cond_t * condVar, int pid){
    if(condVar->size == MAX_COND_VAR_QUEUE_SIZE) return;
    int index = (condVar->index + condVar->size)%MAX_COND_VAR_QUEUE_SIZE;
    condVar->queue[index] = pid;
    condVar->size ++;
}

int removeFromCondVarQueue(cond_t * condVar){
    if(condVar->size==0)return -1;
    int pid = condVar->queue[condVar->index];
    condVar->index = (condVar->index + 1) % MAX_COND_VAR_QUEUE_SIZE;
    condVar->size --;
    return pid;
}

void broadcastCondVar(cond_t * condVar){
    int notPreviouslyLocked = pauseScheduler();
    int i,prevMax = condVar->size;
    for (i = 0; i < prevMax;i++){
        signalCondVarK(condVar);
    }
    if(notPreviouslyLocked) unpauseScheduler();
}