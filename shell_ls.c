#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

void main(int argc, char* argv[]){

    DIR *direccion;
    struct dirent *elem;

    char *p = getenv("PWD");
    direccion = opendir((const char*) p);

    while ((elem = readdir(direccion)) != NULL)
        printf("%s \t", elem->d_name);

    printf("\n");
}
