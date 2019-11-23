#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0

int obtenerOpcion() {
	int opcion;
	printf("\nOpción: \n->");
	scanf("%d%*c", &opcion);
	return opcion;
}

void limpia() {
	system("clear");
}

void esperar() {
	printf("\n<------------------------------>\n");
	printf("\nPresione enter para continuar...");
	scanf("%*c");
	printf("\n");
}
