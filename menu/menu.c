#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"
#define limpia() printf("\033[H\033[J")
#define True 1
#define False 0

//
//funciones
//
char *gets(char *s);
int menuPrincipal();
void wait();
//
//Main
//
int main(int argc, char **argv){
	int m = 1;
	//---------------------------------------------------------------------------------
	do{
		m = menuPrincipal();
		switch(m){
			case 1:
                print("Memoria Compartida");
				break;
			case 2:
				printf("Semaforos");
				break;
			case 3:
				printf("Sellañes entre procesos");
				break;
			default:
				if(m != 0)
					printf("\n Opcion no disponible \n");
				else
					limpia();
					printf("Saliendo...\n");
				break;
		}
	}while(m != 0);
	return 0;
}
//
//funciones
//
/*
 * Menu Principal
 */
int menuPrincipal(){
	int m;
	limpia();
	printf("\n			Bienvenido Programa de procesos\n");
	printf("\n	Seleccione una opción:\n");
	printf("\n<---------------------->\n\n");
	printf("1)Memoria Compartida\n");
	printf("2)Semaforos\n");
	printf("3)Señales\n");
	printf("\n<---------------------->\n\n");
	printf("\n0)Salir.\n");
	printf("\n-> ");
	scanf("%d",&m);
    limpia();
	return m;
}
//Espera
void wait(){
	int c;
	printf("\n<---------------------->\n");
	printf("Continuar...");
	scanf("%d",&c);
}
