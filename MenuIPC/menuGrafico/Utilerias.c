#include "Utilerias.h"
#include <stdio.h>
#include <stdlib.h>

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