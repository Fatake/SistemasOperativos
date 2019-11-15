#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

int salir = 1;

void signalFuncion(int signal){
    printf("\n\n\nPrecionaste ctrl c\n\n\n");
	salir = 0;
	//(void) signal(SIGINT, SIG_DFL);
}

int main(){
	(void) signal(SIGINT, signalFuncion);
	printf("Acctiones ctrl c\n");
	while(salir != 0) {
		printf("Hello World!\n\n");
	}

	printf("Saliendo\n\n");
	return(0);
}
