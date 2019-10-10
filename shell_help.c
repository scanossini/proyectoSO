#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  printf(" ----------------------------------------------------------------------------- \n");
  printf("|                  Shell de comandos creada por los alumnos                   |\n");
  printf("|                    Stefano Canossini y Luciano Pacione.                     |\n");
  printf("|                                                                             |\n");
  printf("| IMPORTANTE: Luego de cada comando debe incluirse un espacio vacio.          |\n");
  printf("| Los siguientes comandos estan incluidos:                                    |\n");
  printf("|                                                                             |\n");
  printf("| cat [nombre]: muestra el contenido del archivo especificado.                |\n");
  printf("| rem [nombre]: elimina el archivo o directorio especificado.                 |\n");
  printf("| mkdir [nombre]: crea un nuevo directorio con el nombre especificado         |\n");
  printf("| touch [nombre]: crea un nuevo archivo con el nombre especificado.           |\n");
  printf("| chmod [nombre permisos] cambia los permisos sobre el archivo                |\n");
  printf("| especificado. Los permisos se deben ingresar en octal (3 digitos).          |\n");
  printf("| ls: muestra el contenido del directorio actual.                             |\n");
  printf("| clear: limpia el contenido de la pantalla.                                  |\n");
  printf("| exit: termina la ejecucion de la shell.                                     |\n");
  printf("| help: muestra esta pantalla de ayuda.                                       |\n");
  printf(" ----------------------------------------------------------------------------- \n");
  return 0;
}

