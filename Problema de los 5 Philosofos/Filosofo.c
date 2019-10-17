#include "ipcfunctions.h"
#include <stdio.h>

//Macros y Definiciones
#define N 5
#define left (N+i-1)%N
#define right (N+i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

//Tipos
typedef int semaforo;

int estado[N];
semaforo mutex;
semaforo sem[N];
int i;

void filosofo(int numeroFilosofo);
void takeForks(int i);
void putForks(int i);
void comprueba(int i);

/*
 * Main
 */
int main(int argc, char **argv){
	printf("give me a bottle of rum!\n");
	//inicializar el estado en pensando
	for(i = 0; i < N ; i++)
		estado[i] = 0;
	//Semaforo de mutex
	mutex = creaSemaforo(0,1);
	//Semaforos para cada Filosofo
	for(i = 0; i < N ; i++)
		sem[i] = creaSemaforo(0,0);
	
	printf("\nSemaforo mutex : %d\n",mutex);
	for ( i = 0; i < N; i++)
		printf("Semaforo numero %d : %d\n",(i+1),sem[i]);
	
	for ( i = 0; i < N; i++)
		destruyeSemaforo(sem[i],0);
	destruyeSemaforo(mutex,0);
	
	return 0;
}
//
//Funciones
//
void filosofo(int numeroFilosofo){
	
}
void takeForks(int i);
void putForks(int i);
void comprueba(int i);