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
int  menuMemoria(int shmid,int valor);
void wait();
//
//Main
//
int main(int argc, char **argv){
	int m = 1, mo, shmid = 0,aux = 0;
	int *entero ;
	//---------------------------------------------------------------------------------
	do{
		m = menuPrincipal();
		switch(m){
			case 1: //Memoria Compartida
				do{
					mo = menuMemoria(shmid, aux);
					switch(mo){
						case 1://Crea Memoria Compartida
							shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
							entero = (int *) shmat(shmid,0,0);
							aux = *entero = 0;
							printf("Memoria Creada \n shmid: %d valor: %d",shmid,*entero);
							wait();
						break;
						case 2://Modifica Memoria
							if(shmid != 0){
								printf("\nshmid: %d valor: %d",shmid,*entero);
								printf("Ingrese un nuevo valor\n->");
								scanf("%d",entero);
								aux = *entero;
							}else
								printf("No a creado una memoria u.u");
							wait();
						break;
						case 3:
							if(shmid != 0){
								printf("\nshmid: %d valor: %d",shmid,*entero);
								shmctl(shmid,IPC_RMID,0);
								aux = 0;
							}else
								printf("No a creado una memoria u.u");
							wait();

						break;
					}
				}while( mo != 0);
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
/*
 * Menu memoria compartida
 */
int menuMemoria(int shmid,int valor){
	int m;
	limpia();
	printf("\n			Menu Memoria\n");
	printf("\n  Seleccione una opción:\n");
	if( shmid != 0)
		printf("\n  Shmid: %d, Valor: %d",shmid,valor);
	
	printf("\n<---------------------->\n\n");
	printf("1)Crear memoria\n");
	printf("2)Modificar valor\n");
	printf("3)Borrar Memoria\n");
	printf("\n<---------------------->\n\n");
	printf("\n0)Menu Principal.\n");
	printf("\n-> ");
	scanf("%d",&m);
    limpia();
	return m;
}
