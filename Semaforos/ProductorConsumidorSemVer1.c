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
int d=0;
//Main

int Prod(int memo[], int indice){
			semDecre(semempty);
			semDecre(semmutex);
			
			memo[indice]=1;
			d++;
			write(1,"PRODUCE| ",sizeof("PRODUCE"));
			printf("LUGARES OCUPADOS EN BUFFER =");
			for(int i=0;i<5;i++)
				printf("%d",memo[i]);
			printf("\n\n");
			
			semIncre(semmutex);
			semIncre(semfull);
	return *memo;
	}
	
int Consum(int memo[],int indice){
			semDecre(semfull);
			semDecre(semmutex);
			
			memo[indice]=0;
			d--;
			write(1,"CONSUME| ", sizeof("CONSUME"));
			printf("LUGARES OCUPADOS EN BUFFER =");
			for(int i=0;i<5;i++)
			printf("%d",memo[i]);
			printf("\n\n");
			
			semIncre(semmutex);
			semIncre(semempty);
	return *memo;
	}

int main(int argc, char **argv){
	int pid;
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	int Mem[]={0,0,0,0,0,0};
	
	semmutex = creaSemaforo(0,1);	semfull = creaSemaforo(0,0);	semempty = creaSemaforo(0,5);

	pid = fork();
	while(True){
		if(pid == 0){
			//write(1,"C", sizeof("C"));
			*Mem=Consum(Mem,d);
			
		}else{
			//write(1,"P",sizeof("P"));
			*Mem=Prod(Mem,d);
			
			
		}
	}

	return 0;
}
