#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "semaforos.h"
//
int randomLotoUp(int lower, int upper,int count);
//Main
int main(int argc, char **argv){
	char c [5];

	int pid,d;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);

	int *contador = (int *) shmat(shmid,0,0);
	*contador = 1;

	int semmutex = creaSemaforo(0,1);//Aumenta Contador e imprime

	pid = fork();

	while(1){
		if(pid == 0){
			srand(time(0)+100);
			write(1,"Proceso Hijo", sizeof("Proceso Hijo"));
			semDecre(semmutex);
			(*contador) --;
			printf(": %d\n",*contador);
			semIncre(semmutex);
			for (int j = 0; j<randomLotoUp(1,50000000,1000);j++)
				d = j*randomLotoUp(1,50000000,1);
		}else{
			srand(time(0)+50);
			write(1,"Proceso Padre",sizeof("Proceso Padre"));
			semDecre(semmutex);
			(*contador) ++;
			printf(": %d\n",*contador);
			semIncre(semmutex);
			for (int j =0; j<randomLotoUp(1,50000000,1000);j++)
				d = j*randomLotoUp(1,50000000,1);
		}
	}

	return 0;
}
//Random
int randomLotoUp(int lower, int upper,int count) {
	int i, num;
	for (i = 0; i < count; i++)
		num = (rand() % (upper - lower + 1)) + lower;
	return num;
}
