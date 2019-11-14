#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define N 5
#define M 6

pthread_t hiloAscensor;
pthread_t hilosPersonas[N];

sem_t personasEnAscensorSubiendo;
sem_t personasEnAscensorBajando;
sem_t puertasAbiertasParaSubir[N];
sem_t puertasAbiertasParaBajar[N];
sem_t personasSuben[N];
sem_t personasBajan[N];

void* funcAscensor(void* args);
void* funcPersona(void* args);

int personasRestantes = N;
int id[N];

int main(int argc, char const *argv[]) {

    sem_init(&personasEnAscensorBajando, 0, 0);
	sem_init(&personasEnAscensorSubiendo, 0, 0);

	srand(time(NULL));

	for(int i = 0; i < N; i++){
        sem_init(&(puertasAbiertasParaSubir[i]),0,0);
        sem_init(&(puertasAbiertasParaBajar[i]),0,0);
        sem_init(&(personasSuben[i]),0,0);
        sem_init(&(personasBajan[i]),0,0);
	}

	printf("Personas: %d, pisos: %d\n\n", N, M);

	for(int j = 0; j < N; j++){//se pasa como parametro al hilo para evitar errores en tiempo de ejecucion
        id[j] = j+1;
	}

	pthread_create(&hiloAscensor, NULL, funcAscensor, NULL);
	for(int p = 0; p < N; p++){
        pthread_create(&hilosPersonas[id[p]], NULL, funcPersona, (void*)p);
	}

	for(int k = 0; k < N; k++){
        pthread_join(hilosPersonas[k], NULL);
	}

	pthread_join(hiloAscensor, NULL);

    return 0;
}

void* funcPersona(void* args){

    int origen = rand() % M;
	int destino = -1;
    int numPersona = (intptr_t) args;

    while(destino == -1 || destino == origen)
        destino = rand() % M;

    sem_post(&personasSuben[origen]);//llamar al ascensor

    printf("Persona %d: pide el ascensor. Origen piso %d, destino piso %d.\n", numPersona, origen, destino);

    sem_wait(&puertasAbiertasParaSubir[origen]);
    sem_post(&personasEnAscensorSubiendo);      //cerrar la puerta
    printf("Persona %d: sube al ascensor \n", numPersona);

    sem_post(&personasBajan[destino]);

    sem_wait(&puertasAbiertasParaBajar[destino]);
    sem_post(&personasEnAscensorBajando);//cerrar la puerta

    printf("Persona %d: baja del ascensor en el piso %d \n", numPersona, destino);
    personasRestantes--;
}

void* funcAscensor(void* args){

    while(personasRestantes > 0){

        for(int i = 0; i < M; i++){
            printf("Ascensor subiendo: piso %d.\n", i);
            while(sem_trywait(&personasBajan[i]) != -1){
                //mientras haya alguien para bajar, se abre la puerta y espera que se baje
                sem_post(&puertasAbiertasParaBajar[i]);
                sem_wait(&personasEnAscensorBajando);
            }
            while(sem_trywait(&personasSuben[i])!=-1){
                //mientras haya alguien para subir, se abre la puerta y espera que suba
                sem_post(&puertasAbiertasParaSubir[i]);
                sem_wait(&personasEnAscensorSubiendo);
            }
            sleep(1);
        }

        for(int j = M-1; j > 0; j--){
            printf("Ascensor bajando: piso %d.\n", j);
            while(sem_trywait(&personasBajan[j]) == 0){
                sem_post(&puertasAbiertasParaBajar[j]);
                sem_wait(&personasEnAscensorBajando);
            }
            while(sem_trywait(&personasSuben[j]) == 0){
                sem_post(&puertasAbiertasParaSubir[j]);
                sem_wait(&personasEnAscensorSubiendo);
            }
            sleep(1);
        }
    }
    printf("No hay mas personas.\n");
    pthread_exit(0);
}
