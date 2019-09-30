#include <stdio.h>
#include <stdlib.h>
#include "semaforos.h"
//Main
int main(int argc, char **argv){
	char c[5];
	int semforo = creaSemaforo(0,1);
	printf("give me a bottle of rum!\n");
	if(semforo < 0){
		printf("No se creo el semaforo");
		exit(1);
	}
	printf("Semid %d \n",semforo);
	printf("Borrrar\n");
	scanf("%s",&c);
	if(!destruyeSemaforo(semforo,0)){
		printf("No se borro el semaforo");
		exit(1);
	}
	printf("Se Borro correctamente\n");

	return 0;
}
