#ifndef CONDVAR_H
#define CONDVAR_H

#define MAX_COND_VAR_NAME_LENGHT 50
#define MAX_COND_VAR_QUEUE_SIZE 50
#define MAX_COND_VAR 30


typedef struct {
    int key;
    int queue[MAX_COND_VAR_QUEUE_SIZE];
    int size;
    int index;
    int mutex;
}cond_t;

void destroyCVK(int key);
void createCondVarsK(int key);
void initCondVarK(cond_t * condVar);
void waitCondVarK(int condVar, int mutex);
void signalCondVarK(int condVar);
int getUsedCondVars(char * names, char * values, int from);
void addToCondVarQueueK(cond_t * condVar, int pid);
int removeFromCondVarQueue(cond_t * condVar);

#endif