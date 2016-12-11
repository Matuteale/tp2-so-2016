#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "philosopher.h"
#define MAX_PHILOSPHERS 8
#define INITIALNUMBER 5
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
char commandPhil = 0;
char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };


void diningPhilosophers() {
	int a = 1;
	mutex = MUTEXKEY;
	createMutex(MUTEXKEY);
	if(philosopherInit() == -1) {
	 	return;
 	}

	 while(1) {
	 	// printString("a");
		 // render();
	 	//sys_sleep(10000);
	 	render();
	 	printString("hola");

	 	getChar(&commandPhil);

		printString(&commandPhil);
	 	switch(commandPhil) {
	 		case 'q': killPhilosophers(); return; break;
	 		case 'w': addPhilosopher(); break;
	 		case 's': removePhilosopher(); break;
	 		case 'p': printPIDs(); break;
	 		default: break;
	 	}
	 	commandPhil = 0;
	 	// clearscreen();
	 }
}

void printPIDs() {
	clearscreen();
	for(int i = 1; i <= philosopherCount; i++) {
		printDec(philosopherPID[i-1]);
		printString("-");
	}
	while(1);
}

void philosopher() {
	int id = ++philosopherCount;
	int aux = 0;
	while(1) {
		// if(id == 3 + aux || id == 3 - aux) {
		sys_sleep(8000);
		takeForks(id);
		sys_sleep(8000);
		putForks(id);
		if(aux == 0) aux++;
		else aux = 0;
		// }

	}
}

void takeForks(int id) {
	if(id >= 8) printString("Se bugeo");
	mutexLock(mutex);
	philosopherState[id-1] = HUNGRY;
	try(id);
	if(philosopherState[id-1] == EATING) {
		mutexUnlock(mutex);
	} else {
		while(philosopherState[id-1] != EATING) {
			// printString("Va a esperar ");
			// printDec(id);
			waitCondVar(canEat[id-1], mutex);
			try(id);
			if(philosopherState[id-1] == EATING) {
				mutexUnlock(mutex);
			}
		}
	}
}

void putForks(int id) {
	mutexLock(mutex);
	philosopherState[id] = THINKING;
	forks[left(id)-1] = -1;
	forks[id-1] = -1;
	try(left(id));
	try(right(id));
	mutexUnlock(mutex);
}

void try(int id) {
	if (philosopherState[id-1] == HUNGRY && philosopherState[left(id)-1] != EATING && philosopherState[right(id)-1] != EATING) {
		philosopherState[id-1] = EATING;
		// render();
		forks[left(id)-1] = id;
		forks[id-1] = id;
		// printString("Va a signalear   ");
		// printDec(id);
		signalCondVar(canEat[id-1]);
	}

}

int left(int id) {
	if(id == 1){
		return philosopherCount;
	}
	return id - 1;
}

int right(int id) {
	if(id == philosopherCount){
		return 1;
	}
	return id + 1;
}

void render() {
	clearscreen();
	printString("Press q to quit\n");
	printString("Press s to add a philosopher or w to remove one\n");
	for(int i = 1; i <= philosopherCount; i++) {
		printString("Philosopher ");
		printDec(i);
		printString(": ");
		printString(stateStrings[philosopherState[i-1]]);
		printString("\n");
		printString("Fork - ");

		if (forks[i-1] == -1)
			printString("Free\n");
		else {
			printString("Owner ");
			printDec(forks[forks[i-1]]);
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
			forks[philosopherCount-1] = -1;
			sys_killProcess(philosopherPID[philosopherCount-1]);
			philosopherPID[philosopherCount-1] = 0;
			destroyCondVars(canEat[philosopherCount-1]);
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
	while(1) {
		mutexLock(mutex);
		if (philosopherState[0] != EATING) {
			initCondVar(&canEat[philosopherCount]);
			pid = sys_addProcess("philo", philosopher, 1);
			printString("PID ");
			printDec(pid);
			while(philosopherPID[i] != 0){i++;};
			philosopherPID[i] = pid;
			if(pid == -1) {
				return -1;
			}
			mutexUnlock(mutex);
			return 0;
		}
		mutexUnlock(mutex);
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
		canEat[i] = condition_key++;
		createCondVars(canEat[i]);
		if(canEat[i] < 0) {
			return -1;
		}
	}
	for(i = 0 ; i < INITIALNUMBER ; i ++) {
		// pid = sys_addProcess("philo", philosopher, 1);
		// philosopherPID[i] = pid;
		// if(pid == -1) {
		// 	killPhilosophers();
		// return -1;
		// }
		addPhilosopher();

	}
	// philosopherCount = INITIALNUMBER;
	return 0;
}

void killPhilosophers() {
	int i = 0;
	// while(philosopherPID[i] > 0) {
	// 		printString("MUERTE");

	// 	sys_killProcess(philosopherPID[i++]);
	// 		printString("MUERTE");

	// }
	// 	printString("MUERTE");
	for(int i = 1; i <= philosopherCount; i++) {
		sys_killProcess(philosopherPID[i-1]);
		destroyCondVars(canEat[i-1]);
	}
	sys_getActivePID(&i);
	destroyMutex(MUTEXKEY);
	sys_killProcess(i);
	while(1);
}