#include <stdio.h>
#include <unistd.h>
//Main
int main(int argc, char **argv){
	int pid,b=-1,*a;
	//
	printf("%i    ", getpid());
	pid = fork();
	printf("%i    ",pid);
	//
	
	a=&b;
		if(pid == 0)
			while(1);
				printf("Proceso Hijo %i",(*a)++);
		else
			while(1);
				printf("Proceso Padre %i",--(*a));

	return 0;
}
