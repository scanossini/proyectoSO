//IMPORTANTE: Se asume que el ascensor debe llegar a destino, no puede parar en otro piso para subir a otra persona

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <math.h>
#include <unistd.h>

pthread_mutex_t ascensor;

//pthread_t hiloAscensor;
pthread_t hiloPersona;

int personas,pisos,opcion;
int* edificio;
int pisoActual, pisoDestino, pisoAscensor;


int obtenerPisoAscensor(){
    for(int i=0; i<pisos; i++){
        if(edificio[i]==1)
            return i+1;
    }
}

void *movimiento(){
    pthread_mutex_lock(&ascensor);
    printf("Ingrese su piso actual: ");
    scanf("%d",&pisoActual);
    pisoAscensor = obtenerPisoAscensor();
    printf("*INFO* **El ascensor esta en el piso %d \n",pisoAscensor);
    int n= fabs(pisoActual-pisoAscensor);
    edificio[pisoAscensor-1]=0;
    sleep(n);
    edificio[pisoActual-1]=1;
    printf("*INFO* **El ascensor esta en el piso %d \n",pisoActual);
    printf("Ingrese a que piso quiere ir: ");
    scanf("%d",&pisoDestino);
    n= fabs(pisoActual-pisoDestino);
    edificio[pisoActual-1]=0;
    sleep(n);
    edificio[pisoDestino-1]=1;
    printf("*INFO* **El ascensor esta en el piso %d \n",pisoDestino);
    pthread_mutex_unlock(&ascensor);
}

int main(){
  //  printf("Ingrese cantidad de personas que usaran el ascensor: \n");
  //  scanf("%d",&personas);
    printf("Ingrese cantidad de pisos que posee el edificio: \n");
    scanf("%d",&pisos);
    edificio = (int*)malloc(pisos*sizeof(int));
    pthread_mutex_init(&ascensor,NULL);
    edificio[0]= 1; //Ascensor comienza en el primer piso
    for(int i=1; i<pisos; i++){
        edificio[i]= 0;
    }
    printf("--- 1 Pedir ascensor --- \n");
    printf("--- 2 Finalizar programa --- \n");
    printf("Opcion: ");
    scanf("%d",&opcion);
    while(opcion != 2){
        pthread_create(&hiloPersona,NULL,&movimiento,NULL);
        pthread_join(hiloPersona,NULL);
        printf("--- 1 Pedir ascensor --- \n");
        printf("--- 2 Finalizar programa --- \n");
        printf("Opcion: ");
        scanf("%d",&opcion);
    }
    exit(1);

}




