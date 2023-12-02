#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> 
#include <unistd.h>

void* fonc(void* arg){
int i;
for(i=0;i<7;i++){
printf("Tache %ld : %d /**/", (intptr_t) arg, i);
usleep(1000000); }}
int main(void)
{
pthread_t tache1, tache2;
pthread_attr_t attr;
struct sched_param param;
pthread_attr_init(&attr);
param.sched_priority = 12;
pthread_setschedparam (pthread_self(), 
SCHED_FIFO, &param); 
//pthread_self() pointe sur le processus en coursd'exécution, à l'occurrence la fonction main()
//le processus main() sera ordonnancé en SHED_FIFO avec une priorité de 12
pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
param.sched_priority = 10;
pthread_attr_setschedparam(&attr, &param); 
pthread_create(&tache1, &attr, fonc,(void*)1); 
// la tâche tache1 créée, sera ordonnancée en SHED_FIFO avec une priorité de 10
param.sched_priority = 7;
pthread_attr_setschedparam(&attr, &param);
pthread_create(&tache2, &attr, fonc, (void*)2); 
// la tâche tache2 créée, sera ordonnancée en SHED_FIFO avec une priorité de 7
pthread_attr_destroy(&attr);
pthread_join(tache1, NULL);
pthread_join(tache2, NULL);
return 0;
}
