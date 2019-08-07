#include <stdio.h>
#include <unistd.h>
//Main
int main(int argc, char **argv){
	int pid;
	if((pid = fork()) == 0)
		write(1,"Proceso Hijo", sizeof("Proceso Hijo"));
	else
		write(1,"Proceso Padre",sizeof("Proceso Padre"));
	
	printf("\n\n");
	return 0;
}
