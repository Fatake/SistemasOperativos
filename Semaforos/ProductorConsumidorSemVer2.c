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


int semmutex, semfull, semempty ;

//Main

int* Prod(int* memo){
			semDecre(semempty);
			semDecre(semmutex);

			(*memo)++;
			write(1,"PRODUCE| ",sizeof("PRODUCE"));
			printf("LUGARES OCUPADOS EN BUFFER = %d\n\n", *memo);

			semIncre(semmutex);
			semIncre(semfull);
	return *memo;
}

int* Consum(int* memo){
			semDecre(semfull);
			semDecre(semmutex);

			(*memo)--;
			write(1,"CONSUME| ", sizeof("CONSUME"));
			printf("LUGARES OCUPADOS EN BUFFER = %d\n\n", *memo);

			semIncre(semmutex);
			semIncre(semempty);
	return *memo;
}

int main(int argc, char **argv){
	int pid;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	int *Mem = (int *) shmat(shmid,0,0);

	*Mem = 0;

	semmutex = creaSemaforo(0,1);	semfull = creaSemaforo(0,0);	semempty = creaSemaforo(0,100);

	pid = fork();
	while(True){
		if(pid == 0){
			//write(1,"CONSUMIDOR", sizeof("CONSUMIDOR"));
			*Mem = Consum(Mem);

		}else{
			//write(1,"PRODUCTOR",sizeof("PRODUCTOR"));
			*Mem = Prod(Mem);


		}
	}

	return 0;
}
