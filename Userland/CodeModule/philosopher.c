#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "philosopher.h"
//#include "philosophersGUI.h"

int left(int i);
int right(int i);
void * philosopher(void * id);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);


int mutex;
int canEat[MAX_PHILOSPHERS];
State philosopherState[MAX_PHILOSPHERS];
int forks[MAX_PHILOSPHERS];
pid_t philosopherPID[MAX_PHILOSPHERS];
int philosopherCount;

void diningPhilosophers() {
	if(philosopherInit() == -1) {
		return;
	}
	printString("Press e to exit");

	printString("Press s to add a philosopher or w to remove one");

	while(1) {
		char c = getchar();
		switch(c) {
			case 'q': killPhilosopher(); return; break;
			case 'w': addPhilosopher(); break;
			case 's': removePhilosopher(); break;
		}
	}
}

void philosopher() {
	int id = philosopherCount++;
	while(1) {
		wait(800);
		takeForks(id);
		wait(3000*id);
		putForks(id);
	}
}

void takeForks(int id) {
	mutexLock(mutex);
	philosopherState[id] = HUNGRY;
	try(id);
	if(philosopherState[id] == EATING) {
		mutexUnlock(mutex);
	} else {
		while(philosopherState[id] != EATING) {
			waitCondVar(canEat[id], mutex);
			try(id);
			if(philosopherState[id] == EATING) {
				mutexUnlock(mutex);
			}
		}
	}
}

void putForks(int id) {
	mutexLock(mutex);
	state[id] = THINKING;
	forks[left(id)] = -1;
	forks[id] = -1;
	try(left(id));
	try(right(id));

	mutexUnlock(mutex)
}

void try(int id) {

	if (state[id] == HUNGRY && state[left(id)] != EATING && state[right(id)] != EATING) {
		state[id] = EATING;
		fork_state[left(id)] = id;
		fork_state[id] = id;
		render();
		signalCondVar(canEat[id]);
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
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
		printf("Fork - ");

		if (forkState[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", forkState[forkState[i]]);
	}

	putchar('\n');
	putchar('\n');
}

int removePhilosopher() {
	if(philosopherCount == 2) {
		return -1;
	}

	while (1) {
		mutexLock(mutex);
		if (state[philosopherCount - 1] != EATING && state[0] != EATING) {

			philosopherPID[philosopherCount - 1] = 0;

			forks[philosopherCount - 1] = -1;

			removeSeat(canEat[philosopherCount - 1]);

			killProcess(philosopherPID[philosopherCount - 1]);

			philosopherCount--;

			mutexUnlock(mutex);

			return 0;

		}

		mutexUnlock(mutex);

	}

	return 0;
}

int philoInitialize() {

	int i;
	int pid;

	philosopherCount = 0;
	mutex = createMutex(MUTEX_KEY);
	if(mutex == -1) {
		return -1;
	}
	if(mutex == -2) {
		fprintf(STDERR, "Error: no mutex available \n");
		return -1;
	}
	for(i = 0 ; i < MAXPHILOS ; i++) {
		fork_state[i] = -1;
	}
	for(i = 0 ; i < MAXPHILOS ; i++) {
		state[i] = 0;
	}
	for(i = 0 ; i < MAXPHILOS ; i++) {
		philosopherPID[i] = 0;
	}
	for(i = 0 ; i < INITIALNUMBER ; i++) {
		canEat[i] = initCondVar(condition_key++);
		if(canEat[i] < 0) {
			fprintf(STDERR, "Error: error creating condition variables.\n");
			return -1;
		}
	}
	for(i = 0 ; i < INITIALNUMBER ; i ++) {
		pid = create_process(&philosopher);
		philosopherPID[i] = pid;
		if(pid == -1) {
			fprintf(STDERR, "Error: couldn't create philosopher process.\n");
			killPhilosophers();
		return -1;
		}
	}
	return 0;
}