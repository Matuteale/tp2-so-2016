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

void initCondVarK(cond_t * condVar) {
    condVar->index = 0;
	condVar->size = 0;
}


void waitCondVarK(cond_t * condVar, int mutex){
    //pauseScheduler();
    condVar->mutex = mutex;
    addToCondVarQueueK(condVar,getCurrentPID());
    changeProcessState(getCurrentPID(),1); //blocked
    mutexUnlockK(&mutex);
    //unlockScheduler();
    yield();
    mutexLockK(&mutex);
}

void signalCondVarK(cond_t * condVar) {
	int pid = removeFromCondVarQueue(condVar);
    if(pid != -1) changeProcessState(pid,0);	//ready
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