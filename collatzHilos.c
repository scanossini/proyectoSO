
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

pthread_t hiloCollatz;
FILE* archivo;

void *collatz(void* num){
    int n = *((int *) num);
    archivo= fopen("Archivo de datos", "w");
    if (archivo==NULL){ //Si no se creo adecuadamente el archivo
        printf("El archivo no se puede abrir \n");
        exit(1);
    }
    else{
        while( n > 1){ //Mientras el valor de n sea mayor a 1
                fprintf(archivo, "%d \n", n);
                if (n > 0){
                    if (n % 2 == 0) {
                        n = n/2;
                    }
                    else {
                        n = 3*n+1;
                    }
                }
        }
        fprintf(archivo, "%d", n); //El valor 1 se debe agregar aparte ya que es la condicion de corte
        fclose(archivo); //Cierro archivo para escritura
    }

}

int main(){
	int numero;
	int aux;
	do{
    	printf("Ingrese un entero positivo: ");
    	scanf("%d", &numero);
    	if (numero < 0)
    		printf("No es un entero positivo.\n");
    }while (numero < 0); //Hasta que no se ingrese un numero mayor a 1

    pthread_create(&hiloCollatz,NULL,collatz,(void*)&numero); //Hilo que me realizara la funcion Collatz
    pthread_join(hiloCollatz,NULL);

    archivo= fopen("Archivo de datos","r"); //Abro archivo para lectura

    printf("\nValores: \n");

    while (feof(archivo)==0){ //Mientras no llegue al final del archivo
        fscanf(archivo,"%d", &aux);
        printf("%d \n",aux);
    }

    fclose(archivo); //Cierro archivo para lectura
    return 0;
}


