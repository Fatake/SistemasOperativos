#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0

int obtenerOpcion() {
  int opcion; 

  printf("\nOpción: ");
  scanf("%d%*c", &opcion);

  return opcion;
}

void limpiarPantalla() {
  system("clear");
}

void esperarEnter() {
  printf("\nPresione enter para continuar...");
  scanf("%*c");
}