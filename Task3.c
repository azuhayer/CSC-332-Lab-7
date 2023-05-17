#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int responseTime;
} Process;

void calculateTimes(Process* processes, int numProcesses, int quantumTime) {
    int remainingBurstTime[numProcesses];
    int currentTime = 0;
    int completedProcesses = 0;

    // Initialize remaining burst time for all processes
    for (int i = 0; i < numProcesses; i++) {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    while (completedProcesses < numProcesses) {
        for (int i = 0; i < numProcesses; i++) {
            if (remainingBurstTime[i] > 0) {
                // Execute the process for the quantum time or remaining burst time, whichever is smaller
                int executeTime = (remainingBurstTime[i] < quantumTime) ? remainingBurstTime[i] : quantumTime;

                // Update completion time
                currentTime += executeTime;
                processes[i].completionTime = currentTime;

                // Update remaining burst time
                remainingBurstTime[i] -= executeTime;

                // Check if the process has completed execution
                if (remainingBurstTime[i] == 0) {
                    completedProcesses++;

                    // Calculate turnaround time, waiting time, and response time
                    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
                    processes[i].responseTime = processes[i].completionTime - processes[i].arrivalTime - processes[i].burstTime;
                }
            }
        }
    }
}

void displayTimes(Process* processes, int numProcesses) {
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    int totalResponseTime = 0;

    printf("Process\tCT\tTT\tWT\tRT\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", i, processes[i].completionTime, processes[i].turnaroundTime, processes[i].waitingTime, processes[i].responseTime);

        totalTurnaroundTime += processes[i].turnaroundTime;
        totalWaitingTime += processes[i].waitingTime;
        totalResponseTime += processes[i].responseTime;
    }

    double avgTurnaroundTime = (double)totalTurnaroundTime / numProcesses;
    double avgWaitingTime = (double)totalWaitingTime / numProcesses;
    double avgResponseTime = (double)totalResponseTime / numProcesses;

    printf("\nAverage TT: %.2lf\n", avgTurnaroundTime);
    printf("Average WT: %.2lf\n", avgWaitingTime);
    printf("Average RT: %.2lf\n", avgResponseTime);
}

int main() {
    int numProcesses;
    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    Process processes[numProcesses];
    for (int i = 0; i < numProcesses; i++) {
        printf("Enter arrival time and burst time for process P%d: ", i);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
    }

    int quantumTime;
    printf("Enter the quantum time: ");
    scanf("%d", &quantumTime);

    calculateTimes(processes, numProcesses, quantumTime);
    displayTimes(processes, numProcesses);

    return 0;
}
