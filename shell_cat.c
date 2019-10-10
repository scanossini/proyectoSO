#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[]){
    FILE* file = fopen(argv[1], "r");
    char c;
    if (file)
        while ((c = getc(file)) != EOF){
            printf("%c", c);
        }

    fclose(file);
}
