#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[]){
    FILE *file = fopen(argv[1], "w");
    fclose(file);
}
