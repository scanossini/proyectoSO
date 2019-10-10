#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void shell_loop();

int main(int argc, char **argv){
  pid_t idHelp = fork();
  char* const* argsHelp = {NULL};
  if(idHelp == 0)
    execv("shell_help", argsHelp);
  wait(NULL);

  shell_loop();
  return 0;
}

char* readLine(){
  int pos = 0;
  char *buffer = malloc(sizeof(char) * 1024);
  char c;

  while (1) {

    c = getchar();

    if (c == '\n') {
        buffer[pos] = '\0';
        return buffer;
    }
    else{
       buffer[pos] = c;
    }
    pos++;
  }
}

void shell_loop(){

  while (1){

    char *line;
    int status;
    int i;

    printf(">> ");
    line = readLine();

    int cant_args = 1;
    int index;
    for (index = 0; index < strlen(line); index++)
      if (line[index] == ' ')
        cant_args++;

    char *args[cant_args];

    args[0] = strtok(line, " ");

    for (i = 1; i<cant_args; i++)
        args[i] = strtok(NULL, " ");

    if (strcmp(args[0], "exit") == 0){
        printf("\nGracias por utilizar la shell!\n");
        return;
    }

    pid_t id = fork();

    if (id == 0){

        if (strcmp(args[0], "help") == 0){
          execv("shell_help", args);
          return;
        }

        if (strcmp(args[0], "mkdir") == 0){
            execv("shell_mkdir", args);
            return;
        }

        if (strcmp(args[0], "rem") == 0){
            execv("shell_rem", args);
            return;
        }

        if (strcmp(args[0], "touch") == 0){
            execv("shell_touch", args);
            return;
        }

        if (strcmp(args[0], "ls") == 0){
            execv("shell_ls", args);
            return;
        }

        if (strcmp(args[0], "cat") == 0){
            execv("shell_cat", args);
            return;
        }

        if (strcmp(args[0], "clear") == 0){
          execv("shell_clear", args);
          return;
        }

        if (strcmp(args[0], "chmod") == 0){
          execv("shell_chmod", args);
          return;
        }

        printf("El comando ingresado no existe.\n");
        return;
    }
    wait(NULL);
  }
}
