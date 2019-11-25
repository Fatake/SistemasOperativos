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
	size_t byte;
	FILE *nuevaCancionArchivo;

	dinicio = dfinal = inicio = final = NULL;
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	strftime(time_string, sizeof(time_string), "%d/%m /%Y  %H:%M:%S" , ptm);
	//coneccion a sochet
	socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero((char *)&server_addr, sizeof(server_addr));
	hp = gethostbyname (DIRECION);
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = PUERTO;

	limpia();
	while(True){
		printf("\n\nElija una opcion\n");
		printf("1) Obtener Canciones\n");
		printf("2) Descargar una cancion\n");
		printf("3) Reproducir\n");
		printf("4) Mostrar las estadisticas de la conexión\n");
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
				write(socketCliente, nombreCancion, sizeof(nombreCancion));
				//Descargando
				
				printf("Descargando.....\n");
				nuevaCancionArchivo = fopen(nombreCancion, "a");

				do{
					read(socketCliente, byte, sizeof(size_t));
					fwrite(&byte, sizeof(byte), 1, nuevaCancionArchivo);
					printf("%d bytes...\n",byte);
				}while (feof(nuevaCancionArchivo) == 0);
				fclose( nuevaCancionArchivo );
				agregaCanciones(&dinicio,&dfinal,nombreCancion);
				printf("Cancion descargada: %s",nombreCancion);

			break;

			case 3:	
				limpia();
				if(dinicio != NULL){
					do{
						muestraCanciones(dinicio);
						printf("Ingrese el nombre de una cancion\n-> ");
						scanf("%s",nombreCancion);
						limpia();
						if(checaExistenciaCancion(dinicio,nombreCancion) == 1){
							break;
						}else{
							printf("Cancion invalida\n");
						}
					} while (True);
					//Reproduce a cancion
					execlp("padsp","padsp","./rep",nombreCancion,NULL);
				}else 
					printf("No a agregado canciones :(\n");
			break;
			
			case 4: 
				printf("La hora en que se conecto el servidor fue: \n");
				printf("%s \n ", time_string);
				printf("\nTiempo transcurrido: %f segundos. \n", ((double)clock() - start) / CLOCKS_PER_SEC);
			break;

			default:
				printf("Opcion invalida");
			break;

		}
	}
	close (socketCliente);//Cierra coneccion con el servidor
	return 0;
}
