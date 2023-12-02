#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t evt;

void* tache1(void *arg){
    int i=0;
    while(i<10){
        printf("La tache %s s'execute\n", (char*) arg);
        sem_post(&evt);
        i++;
    }
    return NULL;
}

void* tache2(void *arg){
    int i=0;
    while(i<10){
        sem_wait(&evt);
        printf("La tache %s s'execute enfin\n", (char*) arg);
        i++;
    }
    return NULL;
}

int main(){
    pthread_t th1, th2;

    sem_init(&evt, 0, 0);

    pthread_create(&th1, NULL, tache1, "1");
    pthread_create(&th2, NULL, tache2, "2");

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    sem_destroy(&evt); // Added: Cleanup the semaphore

    return 0;
}
