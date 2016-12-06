#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "philosopher.h"
#define MAX_PHILOSPHERS 8
#define INITIALNUMBER 2


int left(int i);
int right(int i);
void philosopher();
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);
void killPhilosophers();
int addPhilosopher();
int removePhilosopher();


int mutex;
int condition_key =  CONDKEY;

int canEat[MAX_PHILOSPHERS];
State philosopherState[MAX_PHILOSPHERS];
int forks[MAX_PHILOSPHERS];
pid_t philosopherPID[MAX_PHILOSPHERS];
int philosopherCount;


char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };


void diningPhilosophers() {

	if(philosopherInit() == -1) {
		printString("Press e to exit\n");
	 	return;
 	}
  printString("Press e to exit\n");

	printString("Press s to add a philosopher or w to remove one\n");
	 while(1) {
	 	char c = getChar();
	 	switch(c) {
	 		case 'q': killPhilosophers(); return; break;
	 		case 'w': addPhilosopher(); break;
	 		case 's': removePhilosopher(); break;
	 	}
	 }
}

void philosopher() {
	int id = philosopherCount++;
	while(1) {
		printString("WWWWWWWWWWWWWWWW");
		sleep(800);
		takeForks(id);
		sleep(3000*id);
		putForks(id);
	}
}

void takeForks(int id) {
	mutexLock(&mutex);
	philosopherState[id] = HUNGRY;
	try(id);
	if(philosopherState[id] == EATING) {
		mutexUnlock(mutex);
	} else {
		while(philosopherState[id] != EATING) {
			waitCondVar(&canEat[id], mutex);
			try(id);
			if(philosopherState[id] == EATING) {
				mutexUnlock(&mutex);
			}
		}
	}
}

void putForks(int id) {
	mutexLock(&mutex);
	philosopherState[id] = THINKING;
	forks[left(id)] = -1;
	forks[id] = -1;
	try(left(id));
	try(right(id));
	mutexUnlock(&mutex);
}

void try(int id) {
	if (philosopherState[id] == HUNGRY && philosopherState[left(id)] != EATING && philosopherState[right(id)] != EATING) {
		philosopherState[id] = EATING;
		forks[left(id)] = id;
		forks[id] = id;
		render();
		signalCondVar(&canEat[id]);
	}

}

int left(int id) {

	return (id + philosopherCount - 1) % philosopherCount;

}

int right(int id) {

	return (id + 1) % philosopherCount;

}

void render() {
	for(int i = 0; i < philosopherCount; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philosopherState[i]]);
		printf("Fork - ");

		if (forks[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", forks
				[forks
				[i]]);
	}

	printf("\n");
	printf("\n");
}

int removePhilosopher() {
	if(philosopherCount == 2) {
		return -1;
	}

	while (1) {
		mutexLock(&mutex);
		if (philosopherState[philosopherCount - 1] != EATING && philosopherState[0] != EATING) {
			philosopherPID[philosopherCount - 1] = 0;

			forks[philosopherCount - 1] = -1;


			sys_killProcess(philosopherPID[philosopherCount - 1]);

			philosopherCount--;

			mutexUnlock(&mutex);

			return 0;

		}

		mutexUnlock(&mutex);

	}

	return 0;
}

int addPhilosopher() {
	int pid;
	if(philosopherCount == MAX_PHILOSPHERS) {
		return -1;
	}
	while(1) {
		mutexLock(&mutex);
		if (philosopherState[0] != EATING) {
			canEat[philosopherCount] = initCondVar(canEat[philosopherCount]);
			//pid = sys_addProcess(&philosopher);
			philosopherPID[philosopherCount] = pid;
			if(pid == -1) {
				return -1;
			}
			mutexUnlock(&mutex);
			return 0;
		}
		mutexUnlock(&mutex);
	}
	return 0;
}

int philosopherInit() {

	int i;
	int pid;

	philosopherCount = 0;
	mutex = 0;
	if(mutex == -1) {
		return -1;
	}
	if(mutex == -2) {
		return -1;
	}
	for(i = 0 ; i < MAX_PHILOSPHERS ; i++) {
		forks[i] = -1;
	}
	for(i = 0 ; i < MAX_PHILOSPHERS ; i++) {
		philosopherState[i] = 0;
	}
	for(i = 0 ; i < MAX_PHILOSPHERS ; i++) {
		philosopherPID[i] = 0;
	}
	for(i = 0 ; i < INITIALNUMBER ; i++) {
		canEat[i] = initCondVar(&canEat[i]);
		if(canEat[i] < 0) {
			return -1;
		}
	}
	for(i = 0 ; i < INITIALNUMBER ; i ++) {
		pid = sys_addProcess("philo", philosopher, 1);
		philosopherPID[i] = pid;
		if(pid == -1) {
			killPhilosophers();
		return -1;
		}
	}
	return 0;
}

void killPhilosophers() {

	int i = 0;

	while(philosopherPID[i] > 0) {

		sys_killProcess(philosopherPID[i++]);
	}

}