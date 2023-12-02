#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_WEIGHT 15

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t truck_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t car_cond = PTHREAD_COND_INITIALIZER;

int current_weight = 0;
int waiting_trucks = 0;

void* truck(void* arg){
    pthread_mutex_lock(&bridge_mutex);

    waiting_trucks++;
    while(current_weight + 15 > MAX_WEIGHT){
        pthread_cond_wait(&truck_cond, &bridge_mutex);
    }
    waiting_trucks--;

    current_weight += 15;
    printf("Truck [ID %ld] crossing the bridge. Current weight: %d\n", pthread_self(), current_weight);

    pthread_mutex_unlock(&bridge_mutex);

    sleep(2); // Simulate crossing bridge

    pthread_mutex_lock(&bridge_mutex);
    current_weight -= 15;
    printf("Truck [ID %ld] left the bridge. Current weight: %d\n", pthread_self(), current_weight);

    // Signal the next waiting truck if there is one, otherwise signal cars
    if(waiting_trucks > 0) {
        pthread_cond_signal(&truck_cond); // Give priority to trucks
    } else {
        pthread_cond_broadcast(&car_cond); // If no trucks, signal all cars
    }

    pthread_mutex_unlock(&bridge_mutex);
    return NULL;
}

void* car(void* arg){
    pthread_mutex_lock(&bridge_mutex);

    // Car will wait if the bridge is at capacity or if there is at least one truck waiting
    while(current_weight + 5 > MAX_WEIGHT || waiting_trucks > 0){
        pthread_cond_wait(&car_cond, &bridge_mutex);
    }

    current_weight += 5;
    printf("Car [ID %ld] crossing the bridge. Current weight: %d\n", pthread_self(), current_weight);

    pthread_mutex_unlock(&bridge_mutex);

    sleep(1); // Simulate crossing bridge

    pthread_mutex_lock(&bridge_mutex);
    current_weight -= 5;
    printf("Car [ID %ld] left the bridge. Current weight: %d\n", pthread_self(), current_weight);

    // If a truck is waiting, signal to truck_cond first to give priority
    if(waiting_trucks > 0) {
        pthread_cond_signal(&truck_cond);
    }
    // If no trucks are waiting, signal to car_cond to allow another car to proceed
    else {
        pthread_cond_signal(&car_cond); // Signal the next car instead of broadcasting to all cars
    }

    pthread_mutex_unlock(&bridge_mutex);
    return NULL;
}

int main(){
    pthread_t vehicles[20];

    for(int i = 0; i < 10; i++){
        if(i % 2 == 0) {
            // Create a truck every other iteration to simulate mixed traffic
            pthread_create(&vehicles[i], NULL, truck, NULL);
        } else {
            // Create a car
            pthread_create(&vehicles[i], NULL, car, NULL);
        }
    }

    // Ensure all threads are finished before exiting the program
    for(int i = 0; i < 10; i++){
        pthread_join(vehicles[i], NULL);
    }

    return 0;
}
