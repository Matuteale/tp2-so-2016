#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "philosopher.h"
#define MAX_PHILOSPHERS 8
#define INITIALNUMBER 4
#define MUTEXKEY 9
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
void render();


int mutex;
int condition_key = CONDKEY;

int canEat[MAX_PHILOSPHERS];
volatile State philosopherState[MAX_PHILOSPHERS];
volatile int forks[MAX_PHILOSPHERS];
pid_t philosopherPID[MAX_PHILOSPHERS];
int philosopherCount;
int auxCounter;
int addingPhil = 0;

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };


void diningPhilosophers() {
	mutex = MUTEXKEY;
	createMutex(MUTEXKEY);
	if(philosopherInit() == -1) {
	 	return;
 	}
 	char philControl = 0;
	 while(1) {
	 	render();
	 	philControl = getChar();
	 	printString(&philControl);
	 	switch(philControl) {
	 		case 'q': killPhilosophers(); return; break;
	 		case 'w': addPhilosopher(); break;
	 		case 's': removePhilosopher(); break;
	 		case 'p': printPIDs(); break;
	 	}
	 	philControl = 0;
	 }
}

void printPIDs() {
	clearscreen();
	for(int i = 0; i < MAX_PHILOSPHERS; i++) {
		printDec(philosopherPID[i]);
		printString("-");
	}
	while(1);
}

void philosopher() {
	int id = philosopherCount++;
	int aux = 0;
	while(1) {
		if(philosopherCount > 1 && !addingPhil){
			takeForks(id);
			while(addingPhil);
			putForks(id);
		}
	}
}

void takeForks(int id) {
	while(addingPhil);
	if(id == 8) printString("Se bugeo");
	while(addingPhil);
	mutexLock(mutex);
	while(addingPhil);
	philosopherState[id] = HUNGRY;
	while(addingPhil);
	try(id);
	while(addingPhil);
	if(philosopherState[id] == EATING) {
		while(addingPhil);
		mutexUnlock(mutex);
		while(addingPhil);
	} else {
		while(addingPhil);
		while(philosopherState[id] != EATING) {
			while(addingPhil);
			waitCondVar(canEat[id], mutex);
			while(addingPhil);
			try(id);
			while(addingPhil);
			if(philosopherState[id] == EATING) {
				while(addingPhil);
				mutexUnlock(mutex);
				while(addingPhil);
			}
		}
	}
}

void putForks(int id) {
	while(addingPhil);
	mutexLock(mutex);
	while(addingPhil);
	philosopherState[id] = THINKING;
	while(addingPhil);
	forks[left(id)] = -1;
	while(addingPhil);
	forks[id] = -1;
	while(addingPhil);
	try(left(id));
	while(addingPhil);
	try(right(id));
	while(addingPhil);
	mutexUnlock(mutex);
	while(addingPhil);
}

void try(int id) {
	if (philosopherState[id] == HUNGRY && philosopherState[left(id)] != EATING && philosopherState[right(id)] != EATING) {
		while(addingPhil);
		philosopherState[id] = EATING;
		while(addingPhil);
		forks[left(id)] = id;
		while(addingPhil);
		forks[id] = id;
		while(addingPhil);
		signalCondVar(canEat[id]);
		while(addingPhil);
	}

}

int left(int id) {

	return (id + philosopherCount - 1) % philosopherCount;

}

int right(int id) {

	return (id + 1) % philosopherCount;

}

void render() {
	clearscreen();
	printString("Press q to quit\n");
	printString("Press s to add a philosopher or w to remove one\n");
	for(int i = 0; i < philosopherCount; i++) {
		printString("Philosopher ");
		printDec(i);
		printString(": ");
		printString(stateStrings[philosopherState[i]]);
		printString("\n");
		printString("Fork - ");

		if (forks[i] == -1)
			printString("Free\n");
		else {
			printString("Owner ");
			printDec(forks[forks[i]]);
		printString("\n");
		}

	}

	printf("\n");
	printf("\n");
}

int removePhilosopher() {
	if(philosopherCount == 2) {
		return -1;
	}

	while (1) {
		mutexLock(mutex);
		if (philosopherState[philosopherCount - 1] != EATING && philosopherState[0] != EATING) {
			forks[philosopherCount - 1] = -1;
			sys_killProcess(philosopherPID[philosopherCount - 1]);
			philosopherPID[philosopherCount - 1] = 0;
			philosopherCount--;
			mutexUnlock(mutex);
			return 0;
		}
		mutexUnlock(mutex);
	}
	return 0;
}

int addPhilosopher() {
	int pid, i = 0;
	if(philosopherCount == MAX_PHILOSPHERS) {
		return -1;
	}
	addingPhil = 1;
	while(1) {
		mutexLock(mutex);
		if (philosopherState[0] != EATING) {
			pid = sys_addProcess("Philo", philosopher, 1);
			printString("PID ");
			printDec(pid);
			while(philosopherPID[i] != 0) {i++;}
			philosopherPID[i] = pid;
			if(pid == -1) {
				return -1;
			}
			mutexUnlock(mutex);
			int aux = philosopherCount;
			while(philosopherCount == aux);
			addingPhil = 0;
			return 0;
		}
		mutexUnlock(mutex);
	}
	return 0;
}

int philosopherInit() {

	int i;
	int pid;
	initCondVar();
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
		canEat[i] = condition_key++;
		createCondVars(canEat[i]);
		if(canEat[i] < 0) {
			return -1;
		}
	}
	for(i = 0 ; i < INITIALNUMBER ; i ++) {
		addPhilosopher();

	}
	return 0;
}

void killPhilosophers() {
	int i = 0;
	for(int i = 0; i < philosopherCount; i++) {
		sys_killProcess(philosopherPID[i]);
	}
	sys_getActivePID(&i);
	sys_killProcess(i);
	while(1);
}