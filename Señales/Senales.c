#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int count;

void alarma( ) {
	printf ("Escritura bloqueada después de %d caracteres\n", count);
	exit(0);
}

int main ( ) {

	int p[2];

	char c='x';

	if (pipe (p) < 0 ) {
		perror (pipe) ;
		exit(1) ;
	}

	signal (SIGALRM, alarma) ;

	for (count=0 ;;) {

		alarm(20);
		write(p[1], &c, 1);
		alarm(0);

		if ((++count % 1024) == 0)
			printf("%d caracteres en pipe \n", count);

	}
	return 1;
}
