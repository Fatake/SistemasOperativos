#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "semaforos.h"
#define True 1
#define False 0
#define N 5

/*
 * Semaforos
 */
int semmutex, semfull, semempty ;


/*
 * Productor
 */
int* Prod(int* memo){
	semDecre(semempty);
	semDecre(semmutex);

	(*memo) = 1+randomLotoUp(1,50000000,1);

	printf("P: %d| %d \n\n",semctl ( semfull, 0, GETVAL), *memo);

	semIncre(semmutex);
	semIncre(semfull);
	return *memo;
}

/*
 * consumidor
 */
int* Consum(int* memo){
	semDecre(semfull);
	semDecre(semmutex);

	printf("C: %d| %d \n\n",N-1 - semctl ( semempty, 0, GETVAL), *memo);

	semIncre(semmutex);
	semIncre(semempty);

	return *memo;
}
//Rand
int randomLotoUp(int lower, int upper,int count) {
	int i, num;
	for (i = 0; i < count; i++)
		num = (rand() % (upper - lower + 1)) + lower;
	return num;
}

/*
 * 
 */
int main(int argc, char **argv){
	int pid,d=0;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	int *Mem = (int *) shmat(shmid,0,0);

	*Mem = 0;

	semmutex = creaSemaforo(0,1);
	semfull = creaSemaforo(0,0);
	semempty = creaSemaforo(0,N);
	
	pid = fork();
	while( True )
		if(pid == 0){
			srand(time(0)+500);
			*Mem = Consum(Mem);
			for (int j = 0; j<randomLotoUp(1,50000000,1000);j++)
				d = j*randomLotoUp(1,50000000,1);

		}else{
			srand(time(0)+2500);
			*Mem = Prod(Mem);
			for (int j = 0; j<randomLotoUp(1,50000000,1000);j++)
				d = j*randomLotoUp(1,50000000,1);
		}

	return 0;
}
