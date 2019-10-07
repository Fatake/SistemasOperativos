#include <stdio.h>
#include <string.h>


int main(int argc, char **argv){
	if(argc >= 2)
		if(!strcmp(argv[1],"-m"))
			if(!strcmp(argv[2],"-c")){
				printf("crear memoria\n");
			}else
				printf("Zona de Memoria Compartida");
		else if(!strcmp(argv[1],"-s")){
			printf("Zona de Semaforo");
		}else if(!strcmp(argv[1],"-w")){
			printf("Zona de Mensajes");
		}else if(!strcmp(argv[1],"--help"))
			printf("Parametros admitidos:\n -m Memoria Comparitda \n -s Semaforo \n -w Mensajes\n");
		else
			printf("Comando no encontrado\n Utilice el parametro --help ");
	else
		printf("Comando no encontrado\n Utilice el parametro --help ");
	printf("\n\n");
	return 0;
}
