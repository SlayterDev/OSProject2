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

// Define a CPU
typedef struct {
	unsigned long speed;
	unsigned int memory;
	process queue[50];
	int procCount;
} CPU;

// Global variables for simulation
process processes[50];
CPU cpus[5];
unsigned long maxBurst; // Longest burst time of all processes
double firstRun;
double secondRun;

// Get the longest process
void setMaxBurst() {
	unsigned long max = 0;
	int i;
	for (i = 0; i < 50; i++) {
		if (processes[i].burst > max)
			max = processes[i].burst;
	}

	maxBurst = max;
}

// Generate 50 "random" processes
void generateProcesses() {
	int i;
	for (i = 0; i < 50; i++) {
		process p;
		p.memory = (random() % 2621440); // 0 - 2.5MB
		p.burst = (random() % (400000 - 10000)) + 1000;
		processes[i] = p;
	}
	setMaxBurst();
}

// Create one random process
process getNewProcess() {
	process p;
	p.memory = (random() % 2621440); // .25MB - 8GB
	p.burst = (random() % (400000 - 10000)) + 1000;
	return p;
}

// Reorder queue using selection sort
void reorderQueue(process *queue, int length) {
	int i,j,iMin;

	for (i = 0; i < length-1; i++) {
		iMin = i;
		for (j = i+1; j < length; j++) {
			if (queue[j].burst < queue[iMin].burst) {
				iMin = j;
			}
		}

		if (iMin != i) {
			process temp = queue[i];
			queue[i] = queue[iMin];
			queue[iMin] = temp;
		}
	}
}

// Return the CPU with the longest time
unsigned long long getTurnaroundTime() {
	unsigned long long turnaround = 0, sum = 0;
	int i, j;
	for (i = 0; i < 5; i++) {
		sum = 0;
		for (j = 0; j < cpus[i].procCount; j++) {
			sum += cpus[i].queue[j].burst;
		}
		if (sum > turnaround)
			turnaround = sum;
	}

	return turnaround;
}

// Return the turnaround time for the specified CPU
unsigned long long turnaroundTimeForCpu(int cpu) {
	unsigned long long sum = 0;
	int i;
	for (i = 0; i < cpus[cpu].procCount; i++) {
		sum += cpus[cpu].queue[i].burst;
	}

	return sum;
}

// Gets the CPU with the shortest turnaround time by cycles
// Only useful if all CPU's are the same speed
int getShortestQueueByTime() {
	unsigned long long sum = 0;
	int i, min = 0;
	for (i = 0; i < 5; i++) {
		if (turnaroundTimeForCpu(i) < turnaroundTimeForCpu(min))
			min = i;
	}

	return min;
}

void prob1() {
	process waitQueue[50];
	int wp = 0;
	process readyQueue[50];
	int rp = 0;

	timeval t1, t2;

	gettimeofday(&t1, NULL);

	int i = 0;
	unsigned long long runtime = 0;
	process curProcess;
	int baseP = 0;
	do {
		if (runtime % 50 == 0 && i < 50) {
			process p = processes[i];
			if (p.block = malloc(p.memory)) {
				//printf("New Process with burst: %llu\n", p.burst);
				readyQueue[rp] = p;
				rp++;
			}

			if (i == 0)
				curProcess = p;

			i++;
		}

		if (curProcess.burst == runtime) {
			runtime = 0;
			baseP++;
			//printf("Process done\n");

			if (baseP == 50)
				break;

			curProcess = readyQueue[baseP];
			free(curProcess.block);
		}
		runtime++;
	} while (1);

	gettimeofday(&t2, NULL);

	double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    firstRun = elapsedTime;
    printf("elapsedTime: %fms\n", elapsedTime);
}

void prob2() {
	process waitQueue[50];
	int wp = 0;
	process readyQueue[50];
	int rp = 0;

	timeval t1, t2;

	gettimeofday(&t1, NULL);

	int i = 0;
	unsigned long long runtime = 0;
	process curProcess;
	int baseP = 0;
	do {
		if (runtime % 50 == 0 && i < 50) {
			process p = processes[i];
			if (p.block = myMalloc(p.memory)) {
				//printf("New Process with burst: %llu\n", p.burst);
				readyQueue[rp] = p;
				rp++;
			}

			if (i == 0)
				curProcess = p;

			i++;
		}

		if (curProcess.burst == runtime) {
			runtime = 0;
			baseP++;
			//printf("Process done\n");

			if (baseP == 50)
				break;

			curProcess = readyQueue[baseP];
			myFree(curProcess.block);
		}
		runtime++;
	} while (1);

	gettimeofday(&t2, NULL);

	double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    secondRun = elapsedTime;
    printf("elapsedTime: %fms\n", elapsedTime);
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));

	generateProcesses();
	prob1();
	prob2();

	double delta = firstRun - secondRun;
	printf("The delta between the custom allocator\nand system calls is %fms\n", delta);

	return 0;
}