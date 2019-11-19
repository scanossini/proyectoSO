#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define N 7
#define M 7

pthread_t personas[N];
pthread_t ascensorT;

sem_t personasEnAscensorSubiendo;
sem_t personasEnAscensorBajando;

sem_t puertasAbiertasPSubir[N];
sem_t puertasAbiertasPBajar[N];

sem_t persPSubir[N];
sem_t persPBajar[N];

void* ascensor();
void* persona(void* args);

int main () {

    int j;
    sem_init(&personasEnAscensorBajando, 0, 0);
	sem_init(&personasEnAscensorSubiendo, 0, 0);

	srand(time(NULL));

	for(int i = 0; i < N; i++){
        sem_init(&(puertasAbiertasPSubir[i]),0,0);
        sem_init(&(puertasAbiertasPBajar[i]),0,0);
        sem_init(&(persPSubir[i]),0,0);
        sem_init(&(persPBajar[i]),0,0);
	}

	//Inicializo el ascensor
	pthread_create(&ascensorT, NULL, ascensor, NULL);

	//Inicializo todas las personas
	for(int p = 0; p < N; p++){
        pthread_create(&personas[p], NULL, persona, (void*)p);
	}

	//Espero que finalicen todas las personas
	for(int h = 0; h < N; h++){
        pthread_join(personas[h],NULL);
	}

	//Espero finalice el hilo ascensor
	pthread_join(ascensorT, NULL);

    printf("No hay mas personas \n");
    return 0;
}

void controlDescensoDePasajeros(int piso){
    while(sem_trywait(&persPBajar[piso])!=-1){ //Mientras haya alguien para bajar, le abro la puerta y espero que se baje
        sem_post(&puertasAbiertasPBajar[piso]);
        sem_wait(&personasEnAscensorBajando);
    }
}

void controlAscensoDePasajeros(int piso){
    while(sem_trywait(&persPSubir[piso])!=-1){ //Mientras haya alguien para subir, le abro la puerta y espero que suba
        sem_post(&puertasAbiertasPSubir[piso]);
        sem_wait(&personasEnAscensorSubiendo);
    }
}

void* ascensor(){
    int i, piso;

    while(1){

        for(piso = 0; piso <= N-1; piso++){ // Subir
            printf("**INFO*,ascensor ascendiendo, piso actual %d \n", piso);
            controlDescensoDePasajeros(piso);
            controlAscensoDePasajeros(piso);
            sleep(1);
        }

        for(i = N-1; i >= 0; i--){ // Bajar
            printf("**INFO**,ascensor descendiendo, piso actual %d \n", i);
            controlDescensoDePasajeros(i);
            controlAscensoDePasajeros(i);
            sleep(1);
        }
    }
}

void* persona(void* args){
    int id = (intptr_t)args;
    int origen = rand() % M;
	int destino;

	do{
        destino = rand() % M;
	}while(destino == origen); //Genero un random de destino hasta que destino sea distinto de origen


    printf("Soy la persona %d: origen %d, destino %d\n", id, origen, destino);

    sem_post(&persPSubir[origen]);//Llamo al ascensor

    printf("Soy persona %d y pedi el ascensor\n", id);

    sem_wait(&puertasAbiertasPSubir[origen]); // Espera a que llegue
    sem_post(&personasEnAscensorSubiendo); //Se cierra la puerta
    printf("Soy persona %d y me subi al ascensor \n", id);

    sem_post(&persPBajar[destino]); //Interaccion piso al que queres ir

    sem_wait(&puertasAbiertasPBajar[destino]); //Espera que se abra la puerta para salir
    sem_post(&personasEnAscensorBajando); //Cerrar la puerta

    printf("Soy persona %d y me baje del ascensor en el piso %d \n", id, destino);
}
