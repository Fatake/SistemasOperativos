#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//Main
int main(int argc, char **argv){
	int pid;

	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	printf("\n%d \n",shmid);
	int *contador = (int *) shmat(shmid,0,0);
	*contador = 10;

	pid = fork();
	while(1){
		if(pid == 0){

			write(1,"Proceso Hijo", sizeof("Proceso Hijo"));
			(*contador) --;
			printf(": %d\n",*contador);
		}else{
			write(1,"Proceso Padre",sizeof("Proceso Padre"));
			(*contador) ++;
			printf(": %d\n",*contador);
		}
		sleep(2);
	}

	return 0;
}
