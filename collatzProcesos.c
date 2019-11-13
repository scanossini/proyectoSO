#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int collatz(int n){
    if (n > 0){
        if (n % 2 == 0)
            return (n / 2);
        else
            return (3 * n + 1);
    }
    return 0;
}

int main(){
    int n;
    pid_t id;
    int tubo[2];
    pipe(tubo); //Creo pipe
    do{
        printf("Ingrese un entero positivo: ");
        scanf("%d", &n);
        if (n < 0)
            printf("No es un entero positivo.\n");
    } while (n < 0); //Hasta que no se ingrese un numero mayor a 1
    id = fork(); //Creo un proceso hijo
    int aux = n;
    if (id < 0) //Eror
    	exit(-1);
    if (id == 0){ //Proceso hijo se encarga de aplicar el algoritmo
        close(tubo[0]); //Cierro para que no se pueda leer
        write(tubo[1], &aux, sizeof(int));
        while(aux > 1){
            aux = collatz(aux);
            write(tubo[1], &aux, sizeof(int)); //Escribo
        }
        close(tubo[1]); //Cuando termino de escribir cierro para escritura
    }
    else{ //Proceso padre se encarga de mostrar los resultados
    	close(tubo[1]); //Cierro para que no se pueda escribir
    	printf("\nValores: \n");
        while(aux > 1){
            read(tubo[0], &aux, sizeof(int));
            printf("%d \n", aux);
        }
        close(tubo[0]); //Cuando termino cierro para lectura
    }

}
