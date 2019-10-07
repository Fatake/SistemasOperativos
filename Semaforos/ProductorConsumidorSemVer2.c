#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "semaforos.h"
#define True 1
#define False 0

/*
 * Semaforos
 */


int semmutex, semfull, semempty ;

//Main

int* Prod(int* memo){
	write(1,"PRODUCE| ",sizeof("PRODUCE"));
	semDecre(semempty);
	semDecre(semmutex);

	(*memo)++;

	printf("LUGARES OCUPADOS EN BUFFER = %d\n\n", *memo);

	semIncre(semmutex);
	semIncre(semfull);
	return *memo;
}

int* Consum(int* memo){
	write(1,"CONSUME| ", sizeof("CONSUME"));
	semDecre(semfull);
	semDecre(semmutex);

	(*memo)--;

	printf("LUGARES OCUPADOS EN BUFFER = %d\n\n", *memo);

	semIncre(semmutex);
	semIncre(semempty);

	return *memo;
}
int randomLotoUp(int lower, int upper,int count) {
	int i, num;
	for (i = 0; i < count; i++)
		num = (rand() % (upper - lower + 1)) + lower;
	return num;
}

int main(int argc, char **argv){
	int pid;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	int *Mem = (int *) shmat(shmid,0,0);

	*Mem = 0;

	semmutex = creaSemaforo(0,1);
	semfull = creaSemaforo(0,0);
	semempty = creaSemaforo(0,100);

	pid = fork();
	while( True )
		if(pid == 0){
			srand(time(0)+100);
			*Mem = Consum(Mem);
			for (int j = 0; j<randomLotoUp(1,50000000,1000);j++)
				d = j*randomLotoUp(1,50000000,1);

		}else{
			srand(time(0)+200);
			*Mem = Prod(Mem);
			for (int j = 0; j<randomLotoUp(1,50000000,1000);j++)
				d = j*randomLotoUp(1,50000000,1);
		}

	return 0;
}
