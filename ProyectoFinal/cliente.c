#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "utilerias.h"
#include "socket.h"
#include "lisligcan.h"


int main(int argc, char **argv){
	Canciones *inicio,*final;
	Canciones *dinicio,*dfinal;//Estas variables son para la LL de las canciones descargadas
	clock_t start = clock();
	struct timeval 	tv;
	struct tm* ptm;
	struct sockaddr_in server_addr;
	struct hostent *hp;
	char time_string[40];
	char buff[255];
	char nombreCancion[50];
	int socketCliente;
	int cantCanciones,i,op;
	

	dinicio = dfinal = inicio = final = NULL;
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	strftime(time_string, sizeof(time_string), "%d/%m /%Y  %H:%M:%S" , ptm);
	socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero((char *)&server_addr, sizeof(server_addr));
	//Borra la memoria de server
	hp = gethostbyname (DIRECION);
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = PUERTO;

	limpia();
	while(True){
		printf("\nElija una opcion\n");
		printf("1) Optener Canciones\n");
		printf("2) Descargar una cancion\n");
		printf("3) Enviar un archivo al servidor\n");
		printf("4) Reproducir\n");
		printf("5) Mostrar las estadisticas de la conexión\n");
		printf("6) Mostar Canciones decargadas\n");
		printf("\n<-------------------------------------->\n");
		printf("0) Salir u.u\n-> ");
		scanf("%d",&op);
		limpia();
		if (op == 0)
			break;
		
		
		switch(op){ 
			case 1://Optiene canciones del servidor
				connect(socketCliente, 
					(struct sockaddr *) &server_addr,
					sizeof(server_addr));
				read(socketCliente, buff, 255);
				printf("Numero de canciones en Servidor: %s\n",buff);
				cantCanciones = atoi(buff);
				for ( i = 0; i < cantCanciones; i++){
					read(socketCliente, buff, 255);
					printf("Recibiendo cancion: %s\n",buff);
					agregaCanciones(&inicio,&final,buff);	
				}
			break;
				
			case 2://Descargar cancion
				limpia();
				if (inicio == NULL){
					printf("NO se han recibido canciones del servidor\n");
					break;
				}
				
				do{
					muestraCanciones(inicio);
					printf("Ingrese el nombre de una cancion\n-> ");
					scanf("%s",nombreCancion);
					limpia();
					if(checaExistenciaCancion(inicio,nombreCancion) == 1){
						break;
					}else{
						printf("Cancion invalida\n");
					}
					
				} while (True);
				printf("Enviado cancion %s\n",nombreCancion);
				printf("a.a.a.a");
				printf("\n");
				write(socketCliente, nombreCancion, sizeof(nombreCancion));
				//Se sale???
				printf("Esperando descarga...\n");
				agregaCanciones(&dinicio,&dfinal,nombreCancion);
				printf("Descargando.....\n");
				printf("Cancion descargada: %s",nombreCancion);
                /*
				FILE *ap;
				ap = fopen("Himno.l", "w");
				printf("%s",buff);
				fprintf(ap, "%s", buff);
				fclose(ap);
				*/
			break;

			/*
			case 3:
				
			break;
			*/
			case 4:	
				//nombreCancion=NULL;
				do{
				printf("Ingrese el nombre de la cancion descargada a reproducir:\n");
				scanf("%s",nombreCancion);
				printf("Validando existencia de cancion.........\n");
				checaExistenciaCancion(dinicio,nombreCancion);
				if(checaExistenciaCancion(inicio,nombreCancion) == 1){
						break;
					}else{
						printf("Cancion no descargada\n");
					}
				}
				while (True);
				printf("Reproduciendo cancion.......\n");
				/*
				 * 
				 * 
				 * Aqui pues se debe de meter el codigo para que ejecute
				 * el reproductor con la rola
				 * 
				 * PD: NO MMS ME PUSE ANAL Y YA COMPRENDI QUE PEDO
				 * */
			break;
			
			case 5: 
				printf("La hora en que se conecto el servidor fue: \n");
				printf("%s \n ", time_string);
				printf("\nTiempo transcurrido: %f segundos. \n", ((double)clock() - start) / CLOCKS_PER_SEC);
			break;

			case 6:
				muestraCanciones(dinicio);//MUESTRA CANCIONES DESCARGADAS
			break;

			default:
				printf("Opcion invalida");
			break;

		}
	}
	close (socketCliente);//Cierra coneccion con el servidor
	return 0;
}
