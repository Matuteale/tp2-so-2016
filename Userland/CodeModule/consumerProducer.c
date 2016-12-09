#include <stdio.h>
#include <stdlib.h>
#include "consumerProducer.h"
#include <pthread.h>
#include <unistd.h>
#include "shell.h"

#define BUFFER_SIZE 10
#define PCMUTEX 2


void * pcMQ;
char msgBuffer [21];
int buffer[BUFFER_SIZE];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
int loops = 30;

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
	sys_addProcess("producer", producer, 0);
	mutexUnlock(mutexp);
	mutexLock(mutexp);
	sys_addProcess("consumer", consumer, 0);
	mutexLock(mutexp);
	printString("Press e to exit\n");
	while(1){
		//control();
		receiveMessageQ("pcMQ", msgBuffer);
		printString(msgBuffer);
		//control();
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
		sendMessageQ("pcMQ", "Produce ");
		printString("Produce: ");
		printDec(i++);
		printString("\n");
		// sendMessageQ(pcMQ, 'g');
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
		// printString(" F ");
		printDec(tmp);
		// printString(" G ");
		printString("\n");
		// printString(" R ");
		// sendMessageQ("pcMQ", "caca");
		signalCondVar(empty);
		// printString(" Z ");
		mutexUnlock(mutexp);
	}
	return arg;
}

 void control() {
 	int end = 0;
 	// printString("Press q to quit");
 	while(!end) {
 		char c = getChar();
 		printString(c);
 		switch(c) {
// 			case 'a':
// 				prodSleepTime++;
// 			break;

// 			case 'z':
// 				prodSleepTime = --prodSleepTime < 0? 0 : prodSleepTime;
// 			break;

// 			case 's':
// 				consSleepTime++;
// 			break;

// 			case 'x':
// 				consSleepTime = --consSleepTime < 0? 0 : consSleepTime;
// 			break;

 			case 'q':
 				// printString("AA");
 				end = 1;
 			break;
 			default: end = 1; break;
 		}
 	}
 }
