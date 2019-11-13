#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <math.h>
#include <unistd.h>
#include <TDACola.h>

#define PISOS 20
#define CANTMAX 2

struct informacion{
    int desde;
    int hasta;
}

pthread_t hiloSemaforo;

pthread_t hiloPersona1;
pthread_t hiloPersona2;
pthread_t hiloPersona3;
pthread_t hiloPersona4;

pthread_mutex_t permiso;
pthread_mutex_t mutexAscensor;

sem_t pretendientes;
sem_t ascensor;

TCola<struct informacion> personasEnAscensor;
TCola<struct informacion> solicitudes;

int posAscensor;

void asignarPisos(struct informacion mov){
    int numeroD = rand() % 21;
    int numeroH = rand() % 21;
    mov.desde = numeroD;
    mov.hasta = numeroH;
}

void solicitarPiso(struct informacion info){
    pthread_mutex_lock(&permiso);
    sem_post(&pretendientes); //Aumenta la cantidad de personas que pidieron el ascensor
    c_push(&solicitudes,info); //La persona ingresa a la cola de solicitudes
    pthread_mutex_unlock(&permiso);

}

void buscarHastaDondeVoy(struct informacion info){
    int cant = c_size(&solicitudes);
    int i=0;
    struct informacion aux;
    for(i;i<cant;i++){
        aux= c_pull(&solicitudes)//Retiro solicitud de piso
        if(aux.desde>posAscensor && aux.desde<info.desde){ //Si esta en mi camino (entre posAscensor y desde donde sale la solicitud original)
            if(sem_getvalue(&ascensor)<CANTMAX-1) { //Si hay un lugar de sobra en el ascensor, teniendo en cuenta que quien pidio el ascensor,
                                                    //obligatoriamente se subira
                    pthread_mutex_lock(&mutexAscensor);
                    sem_post(&ascensor); //Se sube al ascensor
                    c_push(&personasEnAscensor,aux);
                    sem_wait(&pretendientes); //Una persona menos en la "lista" de pretendientes
                    printf("Se subio al ascensor la persona en el piso %d \n",aux.desde);
                    pthread_mutex_unlock(&mutexAscensor);
            }
        }
        else{ //Si no esta en mi camino
            c_push(&solicitudes,aux); //vuelvo a colocar en la cola
            c_push(&solicitudesFin,auxFin);
        }
    }
}

void *funcPersona(struct informacion mov){
    solicitarPiso(mov);
}


void moverse(struct informacion info){
    buscarHastaDondeVoy(info);
    struct informacion aux= c_pull(&personasEnAscensor); //Retiro la primer persona que se subio al ascensor




}

void *funcAscensor(){
    sem_wait(&pretendientes);
    mutex_lock(&mutexAscensor);
    if(!c_isEmpty(&solicitudes))
        moverse(c_pull(&solicitudes));

}

void main(){

    //Se generan aleatoriamente los valores de los pisos iniciales y destino de cada persona
    struct informacion persona1;
    asignarPisos(persona1);
    struct informacion persona2;
    asignarPisos(persona2);
    struct informacion persona3;
    asignarPisos(persona3);
    struct informacion persona4;
    asignarPisos(persona4);


    posAscensor = 0; //Ascensor arranca en el piso 0

    sem_init(&pretendientes,0,0); //Personas que pidieron el ascensor
    sem_init(&ascensor,0,CANTMAX); //Inicializo el contador de la cantidad de personas que lleva el ascensor

    c_create(&solicitudes);
    c_create(&personasEnAscensor);

    pthread_mutex_init(&permiso,NULL);
    pthread_mutex_init(&mutexAscensor,NULL);

    pthread_create(&hiloPersona1,NULL,funcPersona,persona1);
    pthread_create(&hiloPersona2,NULL,funcPersona,persona2);
    pthread_create(&hiloPersona3,NULL,funcPersona,persona3);
    pthread_create(&hiloPersona4,NULL,funcPersona,persona4);
    pthread_create(&hiloSemaforo,NULL,funcSemaforo,NULL);


    pthread_join(hiloPersona1,NULL);
    pthread_join(hiloPersona2,NULL);
    pthread_join(hiloPersona3,NULL);
    pthread_join(hiloPersona4,NULL);
    pthread_join(hiloSemaforo,NULL);
}
