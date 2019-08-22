#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define limpia() printf("\033[H\033[J")
//
//Estrucctura Nodo
//
typedef struct nodo{
	char dato[350];
	struct nodo *sig;
}nodo;
//
//Prototipos de Funciones
//
//Funciones ordamentales
char *gets(char *s);
void pausa();
int menu();
//Funciones de lista ligada
void borrar(nodo **inicio,nodo **final);
void agregar(nodo **inicio,nodo **final,char * dato);
int mostrar(nodo *inicio);
//Funciones de manejo de memoria
nodo *nele();
//Funciones de Procesos
void procesosActuales(nodo **inicio,nodo **final);
//Funciones que leer archivos
int leerLinea(nodo **inicio,nodo **final,char * nombreArchivo);

//
//Main
//
int main(int argc, char **argv){
	nodo *inicio,*final;
	char dato[100];
	int m=1;
	inicio = final = NULL;
	do{
		limpia();
		m = menu();
		switch(m){
			case 1: //Agregar
				limpia();
				printf("\n       Agregar:\n");
				printf("\n<---------------------->\n Inserte un Dato\n->");
				//(char*)malloc(sizeof(char)*4);
				gets(dato);
				gets(dato);
				agregar(&inicio,&final,dato);
				break;
			case 2: //Borrar
				borrar(&inicio,&final);
				pausa();
				break;
			case 3: //Mostrar
				limpia();
				mostrar(inicio);
				pausa();
				break;
			case 4: //ps -aux
				limpia();
				procesosActuales(&inicio,&final);
				break;
			default:
				if(m!=0)
					printf("\n Opcion no disponible \n");
				else
					limpia();
					printf("Saliendo...\n");
				break;
		}
	}while(m!=0);
	return 0;
}
//Menu
int menu(){
	int seleccionar = 1;
	printf("Programa de Procesos Simulados\n");
	printf("\n<----------------------->\n\n");
	printf("1) Agregar\n");
	printf("2) Eliminar\n");
	printf("3) Mostrar\n");
	printf("4) Procesos\n");
	printf("\n<----------------------->\n");
	printf("0)Salir\n->");
	scanf("%d",&seleccionar);
	return seleccionar;
}
//recerva memoria
nodo *nele(){
	return (nodo *)malloc(sizeof(nodo));
}
//Agregar
void agregar(nodo **inicio,nodo **final, char * dato){
	nodo *nuevo;
	nuevo = nele();
	if(nuevo == NULL){
		printf("No hay memoria disponible!\n");
		exit(1);
	}else{
		//nuevo->dato = (char*)malloc(sizeof(dato));
		strcpy(nuevo->dato, dato);
		nuevo->sig = NULL;
		if(*inicio == NULL){
			*inicio = nuevo;
			*final = nuevo;
		}else{
			(*final)->sig = nuevo;
			*final = nuevo;
		}
	}
}
//Borrar
void borrar(nodo **inicio, nodo **final){
	int b = 0;
	char nod[50];
	nodo *aux1,*aux2,*aux3;
	limpia();
	printf("	Borrar \n");
	if(*inicio == NULL){
		printf("\n\nNo hay lista\n\n");
	}else{
		mostrar(*inicio);
		printf("\n<---------------------->\n");
		printf("Ingrese dato a borrar: \n->");
		gets(nod);
		gets(nod);
		// proceso de eliminacion
		if(*final == *inicio){//si solo hay uno
			if(!strcmp((*inicio)->dato, nod)){
				aux3 = *final;
				*final = *inicio = (*final)->sig;
				free(aux3);
				printf("\nCanción eliminada !!\n");
			}else b=2;
		}else{
			if(!strcmp((*inicio)->dato,nod)){//si es el primero
				aux3 = *inicio;
				*inicio = (*inicio)->sig;
				aux3->sig = NULL;
				free(aux3);
				printf("\nDato borrado !!\n");
			}else{
				aux1 = (*inicio)->sig;
				aux2 = *inicio;
				do{
					if(!strcmp(aux1->dato,nod)){
						//si esta en medio
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
				if(b == 1){
					free(aux1);
					printf("\nDato Borrado !!\n");
				} 
			}
		}
		if(b==2) printf("\n:(...\n");
	}
}
//Mostrar
int mostrar(nodo *inicio){
	nodo *aux; 
	int i = 0;
	aux = inicio;
	printf("	Datos:\n");
	printf("\n<---------------------->\n");
	while(aux != NULL){
		printf("Proceso: %d \n %s",i+1,aux->dato);
		aux = aux->sig;
		i++;
	}
	if(i == 0)
		printf("\nNo hay datos!!\n");
	return i;
}
//
void pausa(){
	printf("\n<---------------------->\n");
	printf("Continuar...");
	getchar();
	getchar();
}
//
//Leer lineas
//
int leerLinea(nodo **inicio,nodo **final,char * nombreArchivo){
	FILE * fp;//Apuntador Archivo
	char * line = NULL;//Line a leer
	size_t len = 0;
	ssize_t read;
	int contadorPaginas = 1;

	fp = fopen(nombreArchivo, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Tamanio: %zu \n", read);
		//printf("%s\n\n", line);
		if(contadorPaginas != 1)
			agregar(inicio,final,line);
		contadorPaginas ++;
	}

	fclose(fp);
	if (line)
		free(line);

	return contadorPaginas;
}
//
//Ejecuta ps
//
void procesosActuales(nodo **inicio,nodo **final){
	int pid;
	
	printf("	Procesos\n");
	printf("\n<---------------------->\n");
	system("ps -aux");
	/*
	pid = fork();
	if(pid == 0)
		system("ps -aux > ps.txt");
	else
		leerLinea(inicio, final, "ps.txt");
	/////////
	Por agregar
	*/
	pausa();
}

