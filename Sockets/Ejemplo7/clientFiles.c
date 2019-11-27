#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1
#define	ERROR	-1
#define	PUERTO	1100

int main(int argc, char *argv[]){
	struct sockaddr_in stSockAddr;
	int sockLen;
	int Res;
	int SocketFD;
	int recibido;
	char buffer[BUFFSIZE];
	char mensaje[80];
	int totalBytesRcvd;
	int bytesRcvd;
	FILE *archivo;
	
	if(argc < 3){
		perror("USO:./clientFiles <archivo a enviar> <direccionIP del servidor>");
		exit(EXIT_FAILURE);
	}


	/*Se abre el archivo a enviar*/
	archivo = fopen(argv[1],"rb");

	/*Se crea el socket*/
	SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*Se verifica la integridad del archivo*/
	if(!archivo){
		perror("Error al abrir el archivo:");
		exit(EXIT_FAILURE);
	}
    
	/*Se verifica la integridad del socket*/
	if (SocketFD == ERROR){
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	/*Se configura la dirección del socket del cliente*/
	memset(&stSockAddr, 0, sizeof stSockAddr);
 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(1100);
	Res = inet_pton(AF_INET, argv[2], &stSockAddr.sin_addr);
 
	sockLen = sizeof(stSockAddr);

	if (0 > Res){
		perror("error: El primer paránmetro no es una familia de direcciónes");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}else if (Res == 0){
		perror("char string (El segundo parmetro no contiene una dirección IP válida");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	if (connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
		perror("Error a la hora de conectarse con el cliente");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	printf("Se ha conectado con el servidor:%s\n",(char *)inet_ntoa(stSockAddr.sin_addr));

	/*Se envia el archivo*/
	while(!feof(archivo)){
		fread(buffer,sizeof(char),BUFFSIZE,archivo);
		if(send(SocketFD,buffer,BUFFSIZE,0) == ERROR)
			perror("Error al enviar el arvhivo:");
	}
	
	read(SocketFD,mensaje,sizeof(mensaje));
	printf("\nConfirmación recibida:\n%s\n",mensaje);
	
	read(SocketFD,mensaje,sizeof(mensaje));
	printf("\nMD5SUM:\n%s\n",mensaje);
	
	fclose(archivo);
	close(SocketFD);

	return 0;
}//End main

