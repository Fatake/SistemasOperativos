#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char **argv){
	printf("give me a bottle of rum!\n");
	int shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
	printf("\n%d \n",shmid);
	int *entero = (int *) shmat(shmid,0,0);
	*entero = 10;
	printf("\n%d\n",*entero);
	return 0;
}
