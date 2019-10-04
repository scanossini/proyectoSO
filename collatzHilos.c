#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

//gcc -pthread collatzHilos.c

void* collatz(void* n){
	int val = *(int*) n;
	if (val > 0){
		if (val % 2 == 0)
			return (void*) (val / 2);
		else
			return (void*) (3 * val + 1);
	}
	return (void*) 0;
}


int main(){
	int valor;
	do{
    	printf("Ingrese un entero positivo: ");
    	scanf("%d", &valor);
    	if (valor < 0)
    		printf("No es un entero positivo.\n");
    }while (valor < 0);

	FILE* archivo;
	archivo = fopen("test.txt", "a");
	pthread_t hiloCollatz;
	
	printf("\nValores: \n");
	while(valor > 1){
		printf("%d \n",valor);
		fprintf(archivo,"%d \n", valor);
		pthread_create(&hiloCollatz, NULL, collatz, &valor);
		pthread_join(hiloCollatz, (void*) &valor);      
	}

}
