#include <cstdio>
#include <cmath>
#include <ctime>
#include <sys/time.h>
#include <cstdlib>
#include "blk.h"

// Define a process
typedef struct {
	unsigned long burst;
	unsigned int memory;
	void *block;
} process;

// Global variables for simulation
process processes[50]; // List of processes
double firstRun;	   // Runtime for malloc() and free()
double secondRun;	   // Runtime for myMalloc() and myFree()

// Generate 50 "random" processes
void generateProcesses() {
	int i;
	for (i = 0; i < 50; i++) {
		process p;
		p.memory = (random() % 2621440); // 0 - 2.5MB
		p.burst = (random() % (400000 - 10000)) + 1000; // Shorter bursts so we don't
		processes[i] = p;								// have to wait ages for the
	}													// simulation
}

// Create one random process
process getNewProcess() {
	process p;
	p.memory = (random() % 2621440); // 0 - 2.5MB
	p.burst = (random() % (400000 - 10000)) + 1000;
	return p;
}

void prob1() {
	process readyQueue[50]; // Processes with memory, ready to run
	int rp = 0;				// Pointer to top of ready queue

	timeval t1, t2; // For time measurment

	gettimeofday(&t1, NULL); // Start the clock

	int i = 0;
	unsigned long long runtime = 0;
	process curProcess; // Current running process
	int baseP = 0;		// Keep track of which process is next
	do {
		if (runtime % 50 == 0 && i < 50) {
			process p = processes[i];
			if (p.block = malloc(p.memory)) { // Try to allocate memory
				readyQueue[rp] = p;			  // Place it on the ready queue
				rp++;
			}

			if (i == 0)
				curProcess = p;

			i++;
		}

		if (curProcess.burst == runtime) { // Process has finished
			runtime = 0;
			baseP++;

			if (baseP == 50) // No more processes
				break;		 // we're done

			free(curProcess.block); 		// Free the memory
			curProcess = readyQueue[baseP]; // Get the next process
		}
		runtime++;
	} while (1);

	gettimeofday(&t2, NULL); // Stop the clock

	double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; // Calculate time
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    firstRun = elapsedTime;
    printf("elapsedTime: %fms\n", elapsedTime);
}

void prob2() {
	process readyQueue[50]; // Processes with memory, ready to run
	int rp = 0;				// Pointer to top of ready queue

	timeval t1, t2; // For time measurment

	gettimeofday(&t1, NULL); // Start the clock

	int i = 0;
	unsigned long long runtime = 0;
	process curProcess; // Current running process
	int baseP = 0;		// Keep track of which process is next
	do {
		if (runtime % 50 == 0 && i < 50) {
			process p = processes[i];
			if (p.block = myMalloc(p.memory)) { // Try to allocate memory
				readyQueue[rp] = p;			    // Place it on the ready queue
				rp++;
			}

			if (i == 0)
				curProcess = p;

			i++;
		}

		if (curProcess.burst == runtime) { // Process has finished
			runtime = 0;
			baseP++;
			
			if (baseP == 50) // No more processes
				break;		 // we're done

			myFree(curProcess.block); 		// Free the memory
			curProcess = readyQueue[baseP]; // Get the next process
		}
		runtime++;
	} while (1);

	gettimeofday(&t2, NULL); // Stop the clock

	double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; // Calculate time
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    secondRun = elapsedTime;
    printf("elapsedTime: %fms\n", elapsedTime);
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));

	generateProcesses();
	prob1();
	prob2();

	double delta = firstRun - secondRun; // Calculate the difference
	printf("The delta between the custom allocator\nand system calls is %fms\n", delta);

	return 0;
}