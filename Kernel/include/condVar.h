#ifndef CONDVAR_H
#define CONDVAR_H

#define MAX_COND_VAR_NAME_LENGHT 50
#define MAX_COND_VAR_QUEUE_SIZE 50
#define MAX_COND_VAR 30


typedef struct {
    int queue[MAX_COND_VAR_QUEUE_SIZE];
    int size;
    int index;
    int mutex;
}cond_t;

void initCondVarK(cond_t * condVar);
void waitCondVarK(cond_t * condVar, int mutex);
void signalCondVarK(cond_t * condVar);
void broadcastCondVar(cond_t * condVar);

#endif