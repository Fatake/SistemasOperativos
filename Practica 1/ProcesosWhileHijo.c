#include <stdio.h>
#include <unistd.h>
//Main
int main(int argc, char **argv){
	int pid;
	pid = fork();
	if(pid == 0)
		while(1)
			write(1,"Proceso Hijo\n", sizeof("Proceso Hijo\n"));
	else
		write(1,"Proceso Padre\n",sizeof("Proceso Padre\n"));

	return 0;
}
