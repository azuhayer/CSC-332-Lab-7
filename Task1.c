#include <stdio.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

void detectDeadlock(int processes, int allocation[][MAX_RESOURCES], int max_resources[])
{
    int i, j;
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
    int safeSequence[MAX_PROCESSES];
    int p[MAX_PROCESSES] = { 0 };
    int counter = 0;
    int flag = 0;

    // Calculate the need matrix
    for (i = 0; i < processes; i++) {
        for (j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = max_resources[j] - allocation[i][j];
        }
    }

    // Calculate the available resources
    for (j = 0; j < MAX_RESOURCES; j++) {
        int sum = 0;
        for (i = 0; i < processes; i++) {
            sum += allocation[i][j];
        }
        available[j] = max_resources[j] - sum;
    }

    // Deadlock detection algorithm
    while (flag == 0) {
        flag = 1;
        for (i = 0; i < processes; i++) {
            if (p[i] == 0) {
                int canExecute = 1;
                for (j = 0; j < MAX_RESOURCES; j++) {
                    if (need[i][j] > available[j]) {
                        canExecute = 0;
                        break;
                    }
                }
                if (canExecute) {
                    // Execute the process and release resources
                    for (j = 0; j < MAX_RESOURCES; j++) {
                        available[j] += allocation[i][j];
                    }
                    safeSequence[counter++] = i;
                    p[i] = 1;
                    flag = 0;
                }
            }
        }
    }

    // Check if the system is in a safe state
    if (counter < processes) {
        printf("System not safe, deadlock can occur.\n");
    } else {
        printf("System is safe. Safe sequence: ");
        for (i = 0; i < processes; i++) {
            printf("P%d ", safeSequence[i]);
        }
        printf("\n");
    }
}

int main()
{
    int processes = 5;
    int allocation[][MAX_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    int max_resources[] = {10, 5, 7};

    detectDeadlock(processes, allocation, max_resources);

    return 0;
}
