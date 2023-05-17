#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_CHAIRS 5

int numWaiting = 0;       // Number of customers waiting
int barberStatus = 0;     // 0 = sleeping, 1 = awake
pthread_mutex_t mutex;    // Mutex for synchronization
pthread_cond_t customer;  // Condition variable for customer
pthread_cond_t barber;    // Condition variable for barber

void* customerThread(void* arg) {
    int customerID = *(int*)arg;
    sleep(rand() % 3 + 1);  // Simulate customer arrival time

    pthread_mutex_lock(&mutex);
    if (numWaiting == NUM_CHAIRS) {
        printf("Customer %d is leaving (no available chairs)\n", customerID);
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }

    numWaiting++;
    printf("Customer %d is waiting\n", customerID);
    pthread_cond_signal(&customer);  // Notify barber that customer is waiting
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    while (barberStatus == 0) {
        pthread_cond_wait(&barber, &mutex);  // Wait for barber to be awake
    }
    numWaiting--;
    printf("Customer %d is getting a haircut\n", customerID);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* barberThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (numWaiting == 0) {
            barberStatus = 0;  // Barber is sleeping
            printf("Barber is sleeping\n");
            pthread_cond_wait(&customer, &mutex);  // Wait for customer to arrive
            barberStatus = 1;  // Barber is awake
            printf("Barber is awake\n");
            pthread_cond_signal(&barber);  // Notify customer that barber is awake
        }
        pthread_mutex_unlock(&mutex);

        // Haircut
        sleep(rand() % 3 + 1);
    }
}

int main() {
    srand(time(NULL));
    pthread_t barberThreadID, customerThreadID;
    int customerIDs[] = {1, 2, 3, 4, 5};

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&customer, NULL);
    pthread_cond_init(&barber, NULL);

    pthread_create(&barberThreadID, NULL, barberThread, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_create(&customerThreadID, NULL, customerThread, &customerIDs[i]);
        sleep(rand() % 2 + 1);  // Time between customer arrivals
    }

    pthread_join(barberThreadID, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&customer);
    pthread_cond_destroy(&barber);

    return 0;
}
