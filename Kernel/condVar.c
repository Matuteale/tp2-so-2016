#include <stdint.h>
#include <scheduler.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <condVar.h>
#include <mutex.h>

void initCondVar(cond_t * condVar, int pid) {
	condVar->index = 0;
	condVar->size = 0;
}


void waitCondVar(cond_t * condVar, int mutex){
    //pause scheduler
    condVar->mutex = mutex;
    addToCondVarQueue(condVar,getCurrentPID());
    changeProcessState(getCurrentPID(),BLOCKED);
    mutexUnlock(mutex);
    //unlockScheduler();
    yield();
    mutexLock(mutex);
}

void signalCondVar(cond_t * condVar) {
	int pid = removeFromCondVarQueue(condVar);
    if(pid != -1) changeProcessState(pid,READY);	
}

void broadcastCondVar(cond_t * condVar){
    //int notPreviouslyLocked=lockScheduler();
    int i;
    int prevMax = condVar->size;
    for (i = 0; i < prevMax;i++){
        signalCondVar(condVar);
    }
    //if(notPreviouslyLocked) unlockScheduler();
}



void addToCondVarQueue(cond_t * condVar, int pid){
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