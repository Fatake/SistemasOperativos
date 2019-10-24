#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include "semaforos.h"
#define limpia() printf("\033[H\033[J")
#define True 1
#define False 0

//
//funciones
//
char *gets(char *s);
int menuPrincipal();
int menuMemoria(int shmid,int valor);
int menusemaforo(int semid, int valor);
void lanzaSenal();
void wait();
//
//Main
//
int main(int argc, char **argv){
	int m = 1, mo, shmid = 0,aux = 0, semid = 0;
	int *entero;
	//---------------------------------------------------------------------------------
	do{
		m = menuPrincipal();
		switch(m){
			case 1: //Memoria Compartida
				do{
					mo = menuMemoria(shmid, aux);
					switch(mo){
						case 1://Crea Memoria Compartida
							if(shmid == 0){
								shmid = shmget(0,sizeof(int),IPC_PRIVATE|IPC_CREAT|0666);
								entero = (int *) shmat(shmid,0,0);
								aux = *entero = 0;
								printf("Memoria Creada \n shmid: %d valor: %d",shmid,*entero);
							}else
								printf("Usted ya tiene una memoria creada u.u\n");
							wait();
						break;
						case 2://Modifica Memoria
							if(shmid != 0){
								printf("\nShmid: %d valor: %d\n",shmid,*entero);
								printf("Ingrese un nuevo valor\n->");
								scanf("%d",entero);
								aux = *entero;
							}else
								printf("No a creado una memoria u.u");
							wait();
						break;
						case 3://Borrar Memoria
							if(shmid != 0){
								printf("\nshmid: %d valor: %d\n",shmid,*entero);
								shmctl(shmid,IPC_RMID,NULL);
								shmid = 0;
								entero = NULL;
								aux = 0;
								printf("Memoria Borrada n.n\n");
							}else
								printf("No a creado una memoria u.u");
							wait();
						break;
					}
				}while( mo != 0);
			break;
			case 2://Semaforo
				 do{
					mo = menusemaforo(semid, aux);
					switch(mo){
						case 1://Crear Semaforo
							if(semid == 0){
								semid = creaSemaforo(0,0);
								if(semid < 0){
									printf("Error al crear un semaforo u.u\n");
									semid = 0;
								}else{
									aux = 0;
									printf("\n Semid : %d, Valor: %d",semid,aux);
								}
							}else
								printf("Ya tiene creado un semaforo u.u\n");
							wait();
						break;
						case 2://Modificar Valor Semaforo
							if(semid != 0){
								printf("Por hacer");
							}else
								printf("No a creado ningun semaforo u.u\n");
							wait();
						break;
						case 3://Borrar Semaforo
							if(semid != 0){
								if(destruyeSemaforo(semid,0) > 0){
									aux = 0;
									semid = 0;
									printf("Semaforo Destruido n.n\n");
								}else
									printf("No se pudo destruir el semaforo");
							}else
								printf("No a creado ningun semaforo u.u\n");
							wait();
						break;
					}
				}while( mo != 0);
			break;
			case 3:
				printf("Sellañes entre procesos");
				signal(SIGINT,lanzaSenal);
				while(True){};
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
/*
 * Menu Semaforo
 */
 int menusemaforo(int semid, int valor){
	int m;
	limpia();
	printf("\n			Menu Semaforo\n");
	printf("\n  Seleccione una opción:\n");
	if( semid != 0)
		printf("\n Semid : %d, Valor: %d",semid,valor);

	printf("\n<---------------------->\n\n");
	printf("1)Crear Semaforo\n");
	printf("2)Modificar valor\n");
	printf("3)Borrar Semaforo\n");
	printf("\n<---------------------->\n\n");
	printf("\n0)Menu Principal.\n");
	printf("\n-> ");
	scanf("%d",&m);
    limpia();
	return m;
}
//
void lanzaSenal(){
	printf("Señal cachada \n");
}
