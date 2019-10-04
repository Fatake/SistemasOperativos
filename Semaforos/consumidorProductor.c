#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"
#define True 1
#define False 0
/*
 * Semaforos
 */
int semmutex, semprodu, semconsu ;
//Main
int main(int argc, char **argv){
	int pid,d;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	int *contador = (int *) shmat(shmid,0,0);
	*contador = 1;

	semmutex = creaSemaforo(0,1);
	semprodu = creaSemaforo(0,0);
	semconsu = creaSemaforo(0,100);

	pid = fork();

	while( True ){
		if(pid == 0){
			write(1,"Proceso Hijo", sizeof("Proceso Hijo"));
			semDecre(semmutex);
			(*contador) --;
			printf(": %d\n",*contador);
			semIncre(semmutex);
			for (int j =0; j<55555555;j++)
				d = j;
		}else{
			write(1,"Proceso Padre",sizeof("Proceso Padre"));
			semDecre(semmutex);
			(*contador) ++;
			printf(": %d\n",*contador);
			semIncre(semmutex);
			for (int j =0; j<55555555;j++)
				d = j;
		}
	}

	return 0;
}
