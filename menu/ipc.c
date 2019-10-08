#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ipcfunctions.h"

/*
 * Main
 */
int main(int argc, char **argv){
	int aux,i;
	if(argc >= 4){
		//Memoria
		if(!strcmp(argv[1],"-m")){
			if(!strcmp(argv[2],"-c")){//Crear Memoria
				aux = createSHM(atoi(argv[3]));
				if(aux > 0)
					printf("Memoria Compartida Creada con el shmid: %d",aux);
				else
					printf("Error al Crear la Memoria Compartida");
			}else if(!strcmp(argv[2],"-d")){//Borrar
				if(!deleteSHM(atoi(argv[3])))
					printf("Memoria con shmid %d Borrada\n",atoi(argv[3]));
				else
					printf("Error al Borrar la Memoria");
			}

		//Memoria
		}else if(!strcmp(argv[1],"-s")){
			if(!strcmp(argv[2],"-c")){//Crear Semáforo
				if( argc > 4 && atoi(argv[3] ) > 0 ){
					for( i = 1; i <= atoi(argv[3]); i++){
						aux = creaSemaforo(0,atoi(argv[4]));
						if(aux > 0)
							printf("Semaforo Creado semid: %d valor: %d\n",aux,atoi(argv[4]));
						else
							printf("Error al Crear el Semaforo %d\n",i);
					}
				}else if(atoi(argv[3]) > 0){
					for( i = 1; i <= atoi(argv[3]); i++){
						aux = creaSemaforo(0,0);
						if(aux > 0)
							printf("Semaforo Creado semid: %d valor: 0\n",aux);
						else
							printf("Error al Crear el Semaforo %d\n",i);
					}
				}else
					printf("Error, #sem debe ser mayor que 0");
			}else if(!strcmp(argv[2],"-d")){//Borrar
				if(destruyeSemaforo(atoi(argv[3]),0) != -1)
					printf("Semid %d Borrado\n",atoi(argv[3]));
				else
					printf("Error al Borrar El semaforo con semid: %d",atoi(argv[3]));
			}

		//Mensajes
		}else if(!strcmp(argv[1],"-w")){
			printf("Zona de Mensajes");

		}else
			printf("Parametro no encontrado\nUtilice --help Para obtener ayuda");
	}else{
		if(argc < 4){
			if(argc > 1 && !strcmp(argv[1],"--help")){
				printf("Programa que crear recursos IPC \n");
				printf("Parametros admitidos:\n");
				printf("-m <Memoria Comparitda> -c <Crear> <Int:Tamanio>\n");
				printf("-m <Memoria Comparitda> -d <Borrar> <Int:shmid>\n");
				printf("-s <Semaforo> -c <Crear> <Int:Cantidad> <Int:Valor Inicial>\n");
				printf("-s <Semaforo> -d <Borrar> <Int:semid>\n");
				printf("-w <Mensajes> -c <Crear> <Parametros>\n");
				printf("-w <Mensajes> -d <Borrar> <mensajeID>\n");
				printf("-s -c <Int:Cantidad> si no se especifica el 4rto parametro el valor defecto es 0\n");
				printf("Ejemplo:\n");
				printf("./ipc -s -c 3 0\n");
				printf("Creara 3 semaforos con valor 0\n");
			}else
				printf("Parametro no encontrado\nUtilice --help Para obtener ayuda");
		}
	}
	printf("\n\n");
	return 0;
}
