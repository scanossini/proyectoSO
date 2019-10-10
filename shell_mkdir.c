#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void main(int argc, char* argv[]){
    mkdir(argv[1], S_IRWXU);
}
