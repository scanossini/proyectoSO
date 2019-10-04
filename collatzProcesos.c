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
    pipe(tubo);
    do{
        printf("Ingrese un entero positivo: ");
        scanf("%d", &n);
        if (n < 0)
            printf("No es un entero positivo.\n");
    } while (n < 0);
    id = fork();
    int aux = n;
    if (id < 0)
    	exit(-1);	
    if (id == 0){
        close(tubo[0]);
	write(tubo[1], &aux, sizeof(int));
        while(aux > 1){
            aux = collatz(aux);
            write(tubo[1], &aux, sizeof(int));
        }
    }
    else{
    	close(tubo[1]);
    	printf("\nValores: \n");
        while(aux > 1){     
	read(tubo[0], &aux, sizeof(int));
	printf("%d \n", aux);
        }
    }

}