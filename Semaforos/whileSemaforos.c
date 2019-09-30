#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"
//Main
int main(int argc, char **argv){
	char c [5];
	int pid,d;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);

	int *contador = (int *) shmat(shmid,0,0);
	*contador = 1;

	int semmutex = creaSemaforo(0,1);//Aumenta Contador e imprime


	printf(" semaforo Mutex: %d \n",semmutex);

	scanf("%s",&c);
	pid = fork();

	while(1){
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
