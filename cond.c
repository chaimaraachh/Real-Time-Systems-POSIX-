#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_cond_t cond;
pthread_mutex_t verrou;

void* tachePeridique(void* periode){
    struct timespec time;
    int i=0;
    clock_gettime(CLOCK_REALTIME, &time);

    while(i<10){
        pthread_mutex_lock(&verrou);
        time.tv_sec = time.tv_sec + (int)(long)periode;
        printf("La tache %s s'execute periodiquement a l'instant %ld secondes\n", "t1", (long)time.tv_sec);
        pthread_cond_timedwait(&cond, &verrou, &time);
        pthread_mutex_unlock(&verrou);
        i++;
    }
    return NULL;
}

int main(void){
    pthread_t tache1;

    // Initialize the condition variable and mutex
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&verrou, NULL);

    pthread_create(&tache1, NULL, tachePeridique, (void*)5); //the tache1 is periodic with a period of 5s

    pthread_join(tache1, NULL);

    // Cleanup the condition variable and mutex
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&verrou);

    return 0;
}
