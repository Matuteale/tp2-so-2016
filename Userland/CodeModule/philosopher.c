#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "philosophers.h"
#include "philosophersGUI.h"

int left(int i);
int right(int i);
void * philosopher(void * id);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);

static const int philosopherCount = 2;
State state[philosopherCount];

pthread_mutex_t mutex;
pthread_mutex_t semaphores[philosopherCount];
pthread_t philosopherThread[philosopherCount];
int philosopherId[philosopherCount];

void * philosopher(void * id) {
	while(1) {
		//Think
		//sleep(10);	
		sleep(randRange(5, 10));

		takeForks(*(int*)id);

		//Eat
		//sleep(10);
		sleep(randRange(5, 10));

		putForks(*(int*)id);
	}
}

void takeForks(int id) {
	pthread_mutex_lock(&mutex);				//Crit zone

	//Set state
	state[id] = Hungry;
	setPhiloState(id, Hungry);
	render();

	test(id);								//Try to acquire forks
	pthread_mutex_unlock(&mutex);			//Crit zone exit
	pthread_mutex_lock(&semaphores[id]);	//Locks if forks not acquired
}

void putForks(int id) {
	pthread_mutex_lock(&mutex);				//Crit zone

	//Set state
	state[id] = Thinking;
	//Think and release forks
	setPhiloState(id, Thinking);
	setForkState(left(id), -1);
	setForkState(id, -1);
	render();

	test(left(id));							//Try to acquire forks for left
	test(right(id));						//Try to acquire forks for right
	pthread_mutex_unlock(&mutex);			//Crit zone exit
}

void test(int id) {
	if (state[id] == Hungry &&				//Philosopher is hungry
		state[left(id)] != Eating &&		//Both philosophers at
		state[right(id)] != Eating) {		//left and right are not eating

		//Set state
		state[id] = Eating;					//Philosopher can eat!
		//Take forks and eat
		setPhiloState(id, Eating);
		setForkState(left(id), id);
		setForkState(id, id);
		render();

		pthread_mutex_unlock(&semaphores[id]);	//Forks acquired, unlock
	}
}

int main(int argc, char ** argv) {
	//Setup
	pthread_mutex_init(&mutex, NULL);

	for (int i = 0; i < philosopherCount; i++) {
		pthread_mutex_init(&semaphores[i], NULL);
		pthread_mutex_lock(&semaphores[i]);		//Philosophers start not having
	}											//ownership of the forks

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (int i = 0; i < philosopherCount; i++) {
		philosopherId[i] = i;
		state[i] = Thinking;
		pthread_create(&philosopherThread[i], &attr, philosopher, &philosopherId[i]);
	}

	printf("running\n");
	getchar();

	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

int left(int i) {
	return (i + philosopherCount - 1) % philosopherCount;
}

int right(int i) {
	return (i + 1) % philosopherCount;
}

int randRange(int min, int max) {
	return rand() % (max - min) + min;
}
