#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define longitud sizeof(struct msgAscensor) - sizeof(long)
#define M 5
#define N 7
#define personaInteresPiso N+1
#define puertasAbiertas N+2
#define KEY 1212

typedef struct msgAscensor{
	long tipo;
	int dato;
} mensaje;

void main(){
	srand(time(NULL));
	int origen = rand() % M + 1;
	int destino;

	do{
        destino = rand() % M + 1;
	}while(destino == origen);

    int idMensaje = msgget(KEY, IPC_CREAT | 0666);

    if(idMensaje == -1){
    	printf("Error al obtener la cola de mensajes\n");
    	exit(-1);
    }

    printf("Persona[%d]: Key = %d\n", getpid(), KEY);
    printf("Persona[%d]: Cola ID = %d\n", getpid(), idMensaje);

    mensaje* msj = (mensaje*)malloc(sizeof(struct msgAscensor));

    if(msj == NULL){
        printf("Mensaje invalido\n");
        exit(-1);
    }

    printf("Persona %d: origen %d, destino %d \n", getpid(), origen, destino);

    msj->tipo = personaInteresPiso;
    msj->dato = origen;

    if(msgsnd(idMensaje, msj, longitud, 0) == -1){
        printf("Error al enviar mensaje\n");
        exit(-1);
    }

    printf("Persona %d: pedi el ascensor \n", getpid());
    fflush(NULL);


    if(msgrcv(idMensaje, msj, longitud, origen, 0) == -1){
        printf("Error al recibir mensaje\n");
        exit(-1);
    }
    printf("Persona %d: me subi al ascensor \n", getpid());
    fflush(NULL);

    msj->tipo = puertasAbiertas;
    msj->dato = getpid();

    if(msgsnd(idMensaje, msj, longitud, 0) == -1){
        printf("Error al recibir mensaje\n");
        exit(-1);
    }

    msj->tipo = personaInteresPiso;
    msj->dato = destino;

    //indico a que piso voy
    if(msgsnd(idMensaje, msj, longitud, 0) == -1){
        printf("Error al enviar mensaje\n");
        exit(-1);
    }

    //espero que el ascensor llegue al piso que indique
    if(msgrcv(idMensaje, msj, longitud, destino, 0) == -1){
        printf("Error al recibir mensaje\n");
        exit(-1);
    }

    msj->tipo = puertasAbiertas;
    msj->dato = getpid();

    if(msgsnd(idMensaje, msj, longitud, 0) == -1){
        printf("Error al enviar mensaje\n");
        exit(-1);
    }

    printf("Persona %d: me baje en el piso %d \n", getpid(), destino);
}
