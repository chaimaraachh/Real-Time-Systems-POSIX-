#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

typedef struct {
    float taille; 
    float poids; 
} type_donneePartagee; 

pthread_mutex_t verrou; // Corrected the typo 'thread_mutex_t' to 'pthread_mutex_t'
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
type_donneePartagee donneePartagee; 

void* tache1(void *arg){
    // Corrected the comment symbol '/'
    type_donneePartagee ma_donneePartagee; 
    int i=0; 
    while(i<10){
        pthread_mutex_lock(&verrou); 
        pthread_cond_wait(&cond, &verrou); 
        ma_donneePartagee = donneePartagee; 
        pthread_mutex_unlock(&verrou); 
        // Corrected the printf syntax 
        printf("La tache %s vient de lire la donnee partagee\n", (char*) arg); 
        usleep(1000000); 
        i++; 
    } 
    return NULL; // Returning NULL at thread function end
}

// Removed the non-comment line 'Exclusion mutuelle & variable condition'
void* tache2(void *arg){
    int i=0; 
    while(i<10){
        pthread_mutex_lock(&verrou); 
        donneePartagee.taille = 100 + rand()%101; 
        donneePartagee.poids = 10 + rand()%101; 
        if(donneePartagee.taille >= 120 && donneePartagee.poids >= 60){
            pthread_cond_signal(&cond); 
        }
        pthread_mutex_unlock(&verrou); 
        // Corrected the printf syntax 
        printf("La tache %s vient de modifier la donnee partagee\n", (char*) arg); 
        usleep(1000000); 
        i++; 
    }
    return NULL; // Returning NULL at thread function end
}

int main(void){ 
    pthread_t th1, th2; 
    pthread_mutex_init(&verrou, NULL); 
    donneePartagee.taille = 100 + rand()%101; 
    donneePartagee.poids = 10 + rand()%101; 
    pthread_create(&th1, NULL, tache1, "1"); 
    pthread_create(&th2, NULL, tache2, "2"); 
    pthread_join(th1, NULL); 
    pthread_join(th2, NULL); 
    return 0; 
}
