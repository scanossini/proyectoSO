#include <stdio.h>
#include "TDACola.h"

int size;

int c_crear(TCola *cola){
	*cola= NULL;
	size=0;
	return 0;
}
int c_destroy(TCola *cola){
	TPosicion aux= *cola;
	TPosicion ultimo= aux;
	while(ultimo != NULL)
		ultimo= ultimo->next;
		aux->next= NULL;
		free(aux);
		aux= ultimo;
	return 0;
}
int c_top(TCola cola){
	return cola->elem;
}
int c_pull(TCola *cola){
	TPosicion aux= *cola;
	*cola= *cola->next;
	int toReturn= aux->elem;
	aux->next= NULL;
	free(aux);
	size--;
	return toReturn;
}
int c_push(TCola *cola, int elem){
	TPosicion pos= (TPosicion) malloc(sizeof(struct celda));
	pos->elem=elem;
	TPosicion aux= *cola;
	while(aux->next != NULL)
		aux= aux->next;
	aux->next= pos;
	pos->next= NULL;
	size++;
	return 0;
}
int c_isEmpty(TCola cola){
	if(cola = NULL)
		return 0;
	else
		return 1;
}
int c_size(TCola cola){
    return size;
}
