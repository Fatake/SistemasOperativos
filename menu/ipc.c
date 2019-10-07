#include <stdio.h>
#include <string.h>

/*
 * Main
 */
int main(int argc, char **argv){
	if(argc >= 4){
		if(!strcmp(argv[1],"-m"))//Memoria
			if(!strcmp(argv[2],"-c"))//Crear
				printf("crear memoria\n");
			else if(!strcmp(argv[2],"-d"))//Borrar
				printf("Borrar Memoria\n");

		else if(!strcmp(argv[1],"-s")){//Semaforo
			printf("Zona de Semaforo");

		}else if(!strcmp(argv[1],"-w")){//Mensajes
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
