#include <stdio.h>
#include <stdlib.h>
#include "consumerProducer.h"
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define PCMUTEX 2


void * pcMQ;
char msgBuffer [21];
int buffer[BUFFER_SIZE];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
int loops = 30;
char commandControl = 0;
int consumerPID;
int producerPID;
int currentPID;

void put(int value) {
	buffer[fill_ptr] = value;
	fill_ptr = (fill_ptr + 1) % BUFFER_SIZE;
	count++;
}

int get() {
	int tmp = buffer[use_ptr];
	use_ptr = (use_ptr +1) % BUFFER_SIZE;
	count--;
	return tmp;
}

int empty = 1, fill = 2;
int mutexp;

void mainProdCons() {
	mutexp = PCMUTEX;
	createMutex(PCMUTEX);
	createCondVars(empty);
	createCondVars(fill);
	openMessageQ("pcMQ");
	mutexLock(mutexp);
	producerPID = sys_addProcess("producer", producer, 1);
	mutexUnlock(mutexp);
	mutexLock(mutexp);
	consumerPID = sys_addProcess("consumer", consumer, 1);
	mutexLock(mutexp);
	printString("Press q to exit\n");
	while(1){
		control();
		receiveMessageQ("pcMQ", msgBuffer);
		printString(msgBuffer);
		if(msgBuffer[0] != 0){
			printString("\n");
		}
		msgBuffer[0] = 0;
	}
}

void * producer(void *arg) {
	int i;
	while (1) {
		mutexLock(mutexp);
		while(count == BUFFER_SIZE) {
			// printString(" Queue llena ");
			waitCondVar(empty, mutexp);
		}
		put(i);
		char msg[15];
		char strInt[10];
		char * strIntAux = itoa(i, strInt);
		char * aux = "Produce: ";
		strcpy(msg, aux);
		strcpy(&(msg[9]), strIntAux);
		sendMessageQ("pcMQ", &msg);
		printString("Produce: ");
		printDec(i++);
		printString("\n");
		sendMessageQ(pcMQ, "P");
		signalCondVar(fill);
		// printString("P");
		// signalCondVar(fill);
		// waitCondVar(empty, mutexp);
		// mutexUnlock(mutexp);
	}
	return arg;
}

void * consumer(void * arg) {
	int i;
	// printString(" A ");
	while(1) {
		// printString(" B ");
		mutexLock(mutexp);
		// printString("C");
		// signalCondVar(empty);
		// waitCondVar(fill, mutexp);
		// printString(" C ");
		while(count == 0) {
			waitCondVar(fill, mutexp);
		}
		// printString(" D ");
		int tmp = get();
		// printString(" E ");
		printf("Consume ");
		char msg[15];
		char strInt[10];
		char * strIntAux = itoa(tmp, strInt);
		char * aux = "Consume: ";
		strcpy(msg, aux);
		strcpy(&(msg[9]), strIntAux);
		sendMessageQ("pcMQ", &msg);
		printString("Produce: ");
		// printString(" F ");
		printDec(tmp);
		// printString(" G ");
		printString("\n");
		// printString(" R ");
		// sendMessageQ("pcMQ", "C");
		signalCondVar(empty);
		// printString(" Z ");
		mutexUnlock(mutexp);
	}
	return arg;
}

 void control() {
	get_input(&commandControl);

	printString(&commandControl);
	switch(commandControl) {
		//case 'a': prodSleepTime++; break;

		//case 'z': prodSleepTime = --prodSleepTime < 0? 0 : prodSleepTime; break;

		//case 's': consSleepTime++; break;

		//case 'x': consSleepTime = --consSleepTime < 0? 0 : consSleepTime; break;

		case 'q': sys_killProcess(producerPID); sys_killProcess(consumerPID); sys_getActivePID(&currentPID); closeMessageQ("pcMQ"); sys_killProcess(currentPID); break;

		default: break;
 	}
}
