#include <stdio.h>
#include <stdlib.h>
#include "consumerProducer.h"
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10


//SACAR
typedef struct MsgAux Msg;
struct MsgAux{
  char msg;
  Msg * next;
};

typedef struct MessageQ {
  unsigned int  id;
  char * name;
  int dead;
  Msg* first;
  Msg* last;
} MessageQ;
//---


void * pcMQ;
char msgBuffer = 0;
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

int empty, fill;
int mutexp;

void mainProdCons() {
	pcMQ = openMessageQ("pcMQ");
	printDec(((MessageQ *)pcMQ)->dead);
	sys_addProcess("producer", producer, 1);
	sys_addProcess("consumer", consumer, 1);
	while(1){
		// printString("Press e to exit\n");
		receiveMessageQ(&pcMQ, &msgBuffer);
		printString("hola");
		if(msgBuffer == 'g'){
			printString("1");
		}
		printString("\n");
		control();
	}
}

void * producer(void *arg) {
	int i;
	void * msgQ;
	for(i = 0; i < loops; i++) {
		mutexLock(&mutexp);
		while(count == BUFFER_SIZE) {
			waitCondVar(&empty, mutexp);
		}
		put(i);
		printString("Produce ");
		printDec(i);
		printString("\n");
		msgQ = getMessageQ("pcMQ");
		sendMessageQ(&msgQ, 'g');
		signalCondVar(&fill);
		mutexUnlock(&mutexp);
	}
	return arg;
}

void * consumer(void * arg) {
	int i;
	for(i = 0; i < loops; i++) {
		mutexLock(&mutexp);
		while(count == 0) {
			waitCondVar(&fill, mutexp);
		}
		int tmp = get();
		printf("Consume ");
		printDec(tmp);
		printString("\n");
		sendMessageQ(pcMQ, 'c');
		signalCondVar(&empty);
		mutexUnlock(&mutexp);
	}
	return arg;
}

 void control() {
 	int end = 0;
 	printString("Press q to quit");
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
 				printString("AA");
 				end = 1;
 			break;
 			default: end = 1; break;
 		}
 	}
 }
