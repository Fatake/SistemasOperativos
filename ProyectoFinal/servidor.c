#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <pwd.h>
#include "utilerias.h"
#include "socket.h"
#include "lisligcan.h"

#define TAMANIOBUFFER 500
#define EXTENSION ".wav"

//
//
//
int connections;
//
//<------------------------------------Prototipos-------------------------------->//
//
int menu();
int leeCancionesDirectorio(Canciones **inicio, Canciones **final);
int iniciaServidor(Canciones *inicio);
int enviaListaCanciones(Canciones *inicio, int SocketConneccion);
int enviaCancion(Canciones *inicio,int SocketConneccion, char* nombre);

/*
 * Main
 */
int main(int argc, char **argv){
	int opcion = 1;
	Canciones *inicio,*final;
	inicio = final = NULL;
	
	limpia();
	printf("\n\tServidor\n");
	leeCancionesDirectorio(&inicio,&final);
	if(muestraCanciones(inicio) == -1){
		exit(EXIT_FAILURE);
	}
	do{
		opcion = menu();
		limpia();
		switch(opcion){
			case 1://Iniciar servidor
				printf("Iniciando servidor...\n");
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
//<----------------------Funciones_de_lista_de_Canciones------------------------->//
/*
 * Menu
 */
int menu(){
	int m = 0;
	printf("\n<------------------------->\n\n");
	printf("1)Iniciar Servidor.\n");
	printf("\n0)Salir. :(\n");
	printf("\n<------------------------->\n\n");
	printf("\n-> ");
	scanf("%d",&m);
	return m;
}

//<-----------------------------Funciones_de_servidor---------------------------->//
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
 * Envia La lista de canciones
 */
int enviaListaCanciones(Canciones *inicio, int SocketConneccion){
	Canciones *aux;
	int canEnviadas = 0;
	//int cantCanciones = cantidadCanciones(inicio);
	char BUFF [255];
	aux = inicio;

	sprintf(BUFF, "%d", cantidadCanciones(inicio));
	
	write(SocketConneccion, BUFF, sizeof(BUFF) );
	printf("Enviando %s canciones\n",BUFF);
	while(aux != NULL){
		//Escritura del socket Cliente
		write (SocketConneccion, aux->nombre, sizeof(aux->nombre));
		printf("Enviado cancion: %s\n",aux->nombre);
		aux = aux->sig;
		canEnviadas ++;
		sleep(2);
	}
	return canEnviadas;
}

/*
 * Envia la cancion selecionada al socket cliente
 */
int enviaCancion(Canciones *inicio,int SocketConneccion, char* nombre){
	Canciones *aux;
	size_t byte;
	int tamArchivo = 0;
	FILE *apuntadorArchivo;
	aux = inicio;

	while(aux != NULL){
		if (!strcmp(aux->nombre,nombre))			
			break;
			/*
			 * Encuentra la cancion y deja el apuntador en
			 * la picicion de la lista
			 * y sale del ciclo
			 */
		aux = aux->sig;
	}
	if ( aux == NULL)	
		return -1;
	apuntadorArchivo = fopen(aux->nombre, "r");//Abre el archivo
	if (apuntadorArchivo == NULL) {
		fputs ("File error",stderr); 
		fclose(apuntadorArchivo);
		exit (EXIT_FAILURE);
	}
	while (feof(apuntadorArchivo) == 0){
		fread(&byte, sizeof(size_t), 1, apuntadorArchivo);
		write(SocketConneccion, byte, sizeof(size_t));
		//tamArchivo ++;
 	}
	//write(SocketConneccion, tamArchivo, sizeof(tamArchivo));//Envia el tamañio de la cancion
	//write (SocketConneccion, apuntadorArchivo, sizeof(apuntadorArchivo));//Envia la cancion
	fclose(apuntadorArchivo);
	return 1;
}

/*
 * Inicia Servidor
 */
int iniciaServidor(Canciones *inicio){
	int SocketDescriptor, escuchador,pid;
	printf("Direccion IP: %s\n",DIRECION);
	printf("Puerto: %d\n",PUERTO);
	printf("Abriendo Socket...\n");
	if ((SocketDescriptor = OpenScoketINET()) == -1){
		printf("Error en el escuchador\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket Descriptor: %d \n",SocketDescriptor);
	printf("Inciando esuchadores...\n");
	escuchador = escucharPeticones(SocketDescriptor,5);
	if (escuchador == -1){
		printf("No se puede escuchar por el puerto\n");
		exit(EXIT_FAILURE);
	}
	do{
		char respuesta[255];
		printf("\n\n\n");
		int connections = acceptarConeccion(SocketDescriptor);
		pid = fork();
		if(pid == 0){//Procesos Hijos
			printf("Coneccion entrante ID: %d\n",connections);
			if (connections == -1){
				printf("Peticion entrante con error...\n");
			}else{//Acciones cuando el cliente se conecta
				printf("Atendiendo coneccion...\n");
				printf("Enviando lista canciones...\n");
				enviaListaCanciones( inicio,connections );
				printf("Esperando respuesta...\n");
				while(read (connections, respuesta, sizeof(respuesta)) <= 0);
				//Lee del socket
				printf("Enviajo Archivo...\n");
				if(enviaCancion( inicio, connections, respuesta) == -1)
					printf("Error al enviar el archivo\n");
				else
					printf("Archivo enviado...\n");	
			}
			printf("Termiando coneccion...\n");
			close (connections);
			exit(EXIT_SUCCESS);
		}
	} while (True);
}