#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
void* fonc(void* arg){
int i;
for(i=0;i<7;i++){
printf("Tache %ld : %d\n",(intptr_t)arg, i);
usleep(1000000); //attendre 1 seconde
} }
int main(void) 
{
pthread_t tache1, tache2;
//déclaration des deux tâches 
pthread_create(&tache1, NULL, fonc, (void*) 1);
//création effective de la tâche tache1 
pthread_create(&tache2, NULL, fonc, (void*) 2);
pthread_join(tache1, NULL); 
//la fonction principale main(void), doit attendre la fin de l'exécution de la tâche tache1 
pthread_join(tache2, NULL);
return 0; }
