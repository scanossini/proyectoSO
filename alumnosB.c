#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t hiloA;
pthread_t hiloB;
pthread_t hiloC;
pthread_t hiloD;
pthread_t hiloE;

sem_t semA;
sem_t semB;
sem_t semC;
sem_t semD;
sem_t semE;

pthread_barrier_t barrera;


#define CURSOS 10
#define ALUMNOS 5

#define tA 3
#define tB 5
#define tC 2
#define tD 3
#define tE 4



void *posgrado(void* n);


int main(){
    pthread_barrier_init(&barrera,NULL,5);
    pthread_create(&hiloA,NULL,posgrado,(void*)tA);
    pthread_create(&hiloB,NULL,posgrado,(void*)tB);
    pthread_create(&hiloC,NULL,posgrado,(void*)tC);
    pthread_create(&hiloD,NULL,posgrado,(void*)tD);
    pthread_create(&hiloE,NULL,posgrado,(void*)tE);
    pthread_join(hiloA,NULL);
    pthread_join(hiloB,NULL);
    pthread_join(hiloC,NULL);
    pthread_join(hiloD,NULL);
    pthread_join(hiloE,NULL);
    pthread_barrier_destroy(&barrera);
}

void cursar(void* n){
    int tiempo = *(int*) n;
    sleep(tiempo);
}

void *posgrado(void* n){
    for(int i=0; i<CURSOS; i++){
        printf("Comenzo a cursar el curso N° %d \n",i+1);
        cursar(&n);
        printf("Finalice el curso N° %d \n",i+1);
        pthread_barrier_wait(&barrera);
    }
}
