#include <stdio.h>
#include <stdlib.h>
#include "consumerProducer.h"
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 100



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
	sys_addProcess("producer",&producer, 1);
	sys_addProcess("consumer",&consumer, 1);
	while(1){
		//control();
	}
}

void * producer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		mutexLock(&mutexp);
		while(count == BUFFER_SIZE) {
			waitCondVar(&empty, mutexp);
		}
		put(i);
		//printf("Produce %d\n", i);
		signalCondVar(&fill);
		mutexUnlock(&mutexp);
	}
}

void * consumer(void * arg) {
	int i;
	for(i = 0; i < loops; i++) {
		mutexLock(&mutexp);
		while(count == 0) {
			waitCondVar(&fill, mutexp);
		}
		int tmp = get();
		//printf("Consume %d\n", tmp):
		signalCondVar(&empty);
		mutexUnlock(&mutexp);
	}
}

// void control() {
// 	int end = 0;

// 	while(!end) {
// 		int c = getchar();

// 		switch(c) {
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

// 			case 'q':
// 				end = 1;
// 			break;
// 		}
// 	}
// }
