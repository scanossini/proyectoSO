#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int countDigs(int i);

int main(int argc, char* argv[]){

    char *ptr2;
    long modeOct;
    modeOct = strtol(argv[2], &ptr2, 8);

    printf("%ld\n", modeOct);
    if(strlen(argv[2]) != 3){
        printf("Error: se debe ingresar un entero octal de 3 digitos.\n");
        exit(-1);
    }

    chmod(argv[1], modeOct);

    int i = access(argv[1], R_OK);
    int j = access(argv[1], W_OK);
    int k = access(argv[1], X_OK);
    printf("Nuevos permisos: lectura: %d, escritura: %d, ejecucion: %d\n", i, j, k);

    return 0;
}
