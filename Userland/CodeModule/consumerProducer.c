// #include <stdio.h>
// #include <stdlib.h>
// #include <semaphore.h>
// #include <signal.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

// #define BUFFER_SIZE 100

// void * producer(void * ctx);
// void * consumer(void * ctx);
// void control();

// int buffer[BUFFER_SIZE];
// int fill_ptr = 0;
// int use_ptr = 0;
// int count = 0;

// void put(int value) {
// 	buffer[fill_ptr] = value;
// 	fill_ptr = (fill_ptr + 1) % BUFFER_SIZE;
// 	count++;
// }

// int get() {
// 	int tmp = buffer[use_ptr];
// 	use_ptr = (use_ptr +1) % BUFFER_SIZE;
// 	count--;
// 	return tmp;
// }

// cond_t empty, fill;
// int mutex;

// void * producer(void *arg) {
// 	int i;
// 	for(i = 0; i < loops; i++) {
// 		mutexLock(&mutex);
// 		while(count == BUFFER_SIZE) {
// 			waitCondVar(&empty, mutex);
// 		}
// 		put(i);
// 		signalCondVar(&fill);
// 		mutexUnlock(&mutex);
// 	}
// }

// void * consumer(void * arg) {
// 	int i;
// 	for(i = 0; i < loops; i++) {
// 		mutexLock(mutex);
// 		while(count == 0) {
// 			waitCondVar(&fill, mutex);
// 		}
// 		int tmp = get();
// 		printf("Consume %d\n", tmp):
// 		signalCondVar(&empty);
// 		mutexUnlock(&mutex);
// 	}
// }

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
