#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

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

void controlDePasajeros(int piso);

void levantarPedidos();

int persDeInteresPorPiso[N+1];
/*Este arreglo representa la cantidad de veces que la puerta se tiene que abrir para un determinado piso
    Ya sea para personas que se bajan o personas que suben
*/
int idMensaje;
mensaje* msj;

void main () {

    int i;
    /*inicializamos el arreglo*/
    for(i=1; i <= N; i++){
        persDeInteresPorPiso[i] = 0;
    }

    idMensaje = msgget(KEY, IPC_CREAT | 0666);
    mensaje* msj = (mensaje*)malloc(sizeof(struct msgAscensor));

	if(msj == NULL){
        printf("Mensaje invalido\n");
        exit(-1);
    }

  	if(idMensaje == -1){
  		printf("Error al obtener la cola de mensajes\n");
    	exit(-1);
  	}

  	printf("***** COLA DE MENSAJE CREADA *****\n");
  	printf("Cola ID: %d y N: %d \n", idMensaje, N);

    /*El Ascensor espera a que lo despierte alguna persona*/

    if(msgrcv(idMensaje, msj, longitud, personaInteresPiso, 0) == -1){
        printf("Error al obtener mensaje\n");
        exit(-1);
    }
  	persDeInteresPorPiso[msj->dato]++;
  	printf("Soy el ascensor y me despertaron \n");
  	printf("Levante un pedido, tengo que ir a %d \n",msj->dato);
  	printf("Comienza el recorrido\n");
  	sleep(1);
  	while(1){
        for(i=1; i <= N; i++){
            printf("ascendiendo, piso: %d \n",i);
            levantarPedidos();
            controlDePasajeros(i);
            sleep(1);
        }
        printf("////// CAMBIO DE DIRECCION //////\n");
        sleep(2);
        for(i=N;i>=1;i--){
            printf("descendiendo, piso: %d \n",i);
            levantarPedidos();
            controlDePasajeros(i);
            sleep(1);
        }
        printf("////// CAMBIO DE DIRECCION //////\n");
        sleep(2);
  	}
  	if (msgctl(idMensaje, IPC_RMID, NULL) == -1) {
		printf("La cola de mensaje no pudo eliminarse\n");
		exit(1);
	}
}
void levantarPedidos(){
    /*Mientras hayagente  que quiera usar el ascensor(mensajes de tipo personaInteresPiso), los leo
        y aumento en 1 la posicion en el arreglo que representa la cantidad de veces
        que la puerta se tiene que abrir para un determinado piso
    */
    while(msgrcv(idMensaje, msj, longitud, personaInteresPiso, IPC_NOWAIT) >= 0){
        printf("Levante un pedido, tengo que ir a %d \n", msj->dato);
        persDeInteresPorPiso[msj->dato]++;
    }
}
void controlDePasajeros(int piso){
    /* Este procedimiento garantiza que todos los requerimientos del piso pasado por parametro se cumplan
    ya sea para bajar o para subir del ascensor. Ya que consulta al arreglo y envia tantos mensajes como numeros
    tenga la posicion(piso). Dicho de otra forma, envia un mensaje para cada persona que quiera bajar/subir del ascensor.
    Tambien controla que la persona que bajo/subio cierre la puerta, esto permite que el ascensor espere antes de volver a arrancar
    */
    while(persDeInteresPorPiso[piso] > 0){
        msj->tipo = piso;
        msj->dato = 0;

        //habilito a que una persona pueda subir
        if(msgsnd(idMensaje, msj, longitud, 0) == -1){
        printf("Error al enviar mensaje\n");
        exit(-1);
        }

        //espero a que cierre la puerta
        if(msgrcv(idMensaje, msj, longitud, puertasAbiertas, 0) == -1){
        printf("Error al recibir mensaje\n");
        exit(-1);
        }
        printf("La persona %d cruzo la puerta, soy el ascensor %d \n ",msj->dato,getpid());
        persDeInteresPorPiso[piso]--; //Esa persona se bajo del ascensor (no necesita ir mas a ese destino(por que ya llego))
                                        // o se subio al ascensor (no esta mas en el piso origen)
    }
}
