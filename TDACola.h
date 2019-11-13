#include <stdlib.h>

#ifndef _COLA
#define _COLA

typedef struct celda {
	int elem;
	struct celda *siguiente;
};

 typedef struct celda *TCola;
 typedef struct celda *TPosicion;

 int c_create(TCola *cola);
 int c_destroy(TCola *cola);
 int c_top(TCola cola);
 int c_pull(TCola cola);
 int c_push(TCola *cola, int elem);
 int c_isEmpty(TCola cola);

#endif //COLA
