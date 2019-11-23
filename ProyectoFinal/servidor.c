#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <pwd.h>
#include "Utilerias.h"
#include "Socket.h"

#define TAMANIOBUFFER 500
#define EXTENSION ".wav"
/*
 * Estructura de canciones
 */
typedef struct canciones{
	char nombre[50];
	struct canciones *sig;
}Canciones;
//
//<----------------Prototipos----------------->//
//
int menu();
Canciones *nele();
int agregaCanciones(Canciones **inicio,Canciones **final, char* nombre);
int muestraCanciones(Canciones *inicio);
int borraCancion(Canciones **inicio,Canciones **final, char* cancion);
int leeCancionesDirectorio(Canciones **inicio, Canciones **final);
int iniciaServidor(Canciones *inicio);

/*
 * Main
 */
int main(int argc, char **argv){
	int opcion = 1;
	Canciones *inicio,*final;
	inicio = final = NULL;
	
	printf("\n\tServidor\n");
	leeCancionesDirectorio(&inicio,&final);
	if(muestraCanciones(inicio) == -1){
		exit(EXIT_FAILURE);
	}
	//<---------------------------------Inicio---------------------------------->//
	do{
		opcion = menu();
		limpia();
		switch(opcion){
			case 1://Mostar Canciones
				printf("Iniciando servidor...");
				iniciaServidor(inicio);
			break;

			default:
				if(opcion != 0)
					printf("\n Opcion no disponible \n");
			break;
		}
	}while(opcion != 0);
	printf("Saliendo...\n");
	return 0;
}
//<-------------------------------Funciones-------------------------------------->//
/*
 * Menu
 */
int menu(){
	int m = 0;
	printf("\n<------------------------->\n\n");
	printf("1)Iniciar Servidor\n");
	printf("\n0)Salir. :(\n");
	printf("\n-> ");
	scanf("%d",&m);
	return m;
}
/*
 * Reserva Memoria para estructura canciones
 */
Canciones *nele(){
	return (Canciones *)malloc(sizeof(Canciones));
}
/*
 * Agrega canciones a la lista ligada
 */
int agregaCanciones(Canciones **inicio,Canciones **final, char* nombre){
	int sal = 0;
	Canciones *nuevo;
	nuevo = nele();
	if(nuevo == NULL){
		printf("No hay memoria disponible!\n");
		exit(EXIT_FAILURE);
	}else{
		strcpy(nuevo->nombre,nombre);
		nuevo->sig = NULL;
		if(*inicio == NULL){
			*inicio = nuevo;
			*final = nuevo;
		}else{
			(*final)->sig = nuevo;
			*final = nuevo;
		}
	}
	return 1;
}
/*
 * Muestra todas las canciones de la lista ligada
 * retorna:
 * * numero de canciones
 * * -1 si no hay canciones
 */
int muestraCanciones(Canciones *inicio){
	Canciones *aux; 
	int i = 1;
	aux = inicio;
	if(aux == NULL){
		printf("No canciones en el servidor\n");
		return -1;
	}
	printf("\n\tCanciones:\n\n");
	while(aux != NULL){
		printf("%d) %s\n",(i++),aux->nombre);
		aux = aux->sig;
	}
	
	return i;
}
/*
 * Borra una cancion de la lista ligada
 * 
 */
int borraCancion(Canciones **inicio,Canciones **final, char* cancion){
	int b = 0;
	Canciones *aux1,*aux2,*aux3;

	if(*inicio == NULL){
		return -1;
	}else{
		if(*final == *inicio){
			if(!strcmp((*inicio)->nombre,cancion)){
				aux3 = *final;
				*final = *inicio = (*final)->sig;
				free(aux3);
			}else 
				b=2;
		}else{
			if(!strcmp((*inicio)->nombre,cancion)){
				aux3 = *inicio;
				*inicio = (*inicio)->sig;
				aux3->sig = NULL;
				free(aux3);
			}else{
				aux1 = (*inicio)->sig;
				aux2 = *inicio;
				do{
					if(!strcmp(aux1->nombre,cancion)){
						aux2->sig = aux1->sig;
						aux1->sig = NULL;
						b = 1;
						break;
					}else{
						if(aux1->sig == NULL){
							b = 2;
							break;
						}else{
							aux1 = aux1->sig;
							aux2 = aux2->sig;
						}
					}
				}while(aux1->sig != NULL );
				if(b == 1)
					free(aux1);
			}
		}
		if(b == 2)
		 return -1;
	}
	return 0;
}
/*
 * Lee canciones del directorio donde el servidor se encuentre
 */
int leeCancionesDirectorio(Canciones **inicio, Canciones **final){
	DIR *dir;
	struct dirent *ent;
	int cantidad = 0;
	char nombre[50];

	/* Empezaremos a leer en el directorio actual */
	dir = opendir (".");

	if (dir == NULL){
		printf("No puedo abrir el directorio");
		exit(EXIT_FAILURE);
	}
	while ((ent = readdir (dir)) != NULL){
		if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
			char*  comparadorExtencion = strstr(ent->d_name, EXTENSION);
			if (comparadorExtencion != NULL){
				cantidad++;
				agregaCanciones(inicio,final,ent->d_name);
			}
		}
	}
	closedir (dir);
	return cantidad;
}
/*
 * Inicia Servidor
 */
int iniciaServidor(Canciones *inicio){
	int SocketDescriptor, escuchador;
	printf("Abriendo Socket..");
	if ((SocketDescriptor = OpenScoketINET()) == -1){
		printf("Error en el escuchador\n");
		exit(EXIT_FAILURE);
	}

	printf("Descriptor: %d \n",SocketDescriptor);
	esperar();
	printf("Inciando esuchadores");
	escuchador = escucharPeticones(SocketDescriptor,5);
	if (escuchador == -1){
		printf("No se puede escuchar por el puerto\n");
		exit(EXIT_FAILURE);
	}
	
}