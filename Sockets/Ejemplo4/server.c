#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "Socket.h"
#include "Utilerias.h"

int main(int argc, char **argv){
	int SocketDescriptor, escuchador;
	/*char *mensaje1[] = {
		"ejemolo mensajes\n",
		"Mas mensajes\n,"
		"Todabia mas mensajes\n"
	};*/
	
	if ((SocketDescriptor = OpenScoketINET()) == -1){
		printf("Error en el escuchador\n");
		exit(EXIT_FAILURE);
	}

	printf("Descriptor: %d \n",SocketDescriptor);
	esperar();
	escuchador = escucharPeticones(SocketDescriptor,5);
	if (escuchador == -1){
		printf("No se puede escuchar por el puerto\n");
		exit(EXIT_FAILURE);
	}
	

	printf("\n\n");
	return 0;
}

