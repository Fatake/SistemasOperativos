#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define limpia() printf("\033[H\033[J")
//
//struct
//
typedef struct nodo{
	char dato[50];
	struct nodo *sig;
}nodo;
//
//funciones
//
char *gets(char *s);
int agrega(nodo **inicio,nodo **final);
nodo *nele();
int mostrar(nodo *inicio);
void borrar(nodo **inicio,nodo **final);
int menu();
//Main
int main(int argc, char **argv){
	nodo *inicio,*final;
	int m=1,f=0,c;
	inicio = final = NULL;
	do{
		limpia();
		m = menu();
		switch(m){
			case 1:
				do{
					limpia();
					f = agrega(&inicio,&final);
				}while(f != 0);
				break;
			case 2:
				borrar(&inicio,&final);
				break;
			case 3:
				limpia();
				mostrar(inicio);
				printf("\n<---------------------->\n");
				printf("Continuar...");
				scanf("%d",&c);
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
	printf("Programa de Listas Ligadas\n");
	printf("<----------------------->\n");
	printf("1)Agregar\n");
	printf("2)Eliminar\n");
	printf("3)Mostrar\n");
	printf("<----------------------->\n");
	printf("0)Salir\n->");
	scanf("%d",&seleccionar);
	return seleccionar;
}
//recerva memoria
nodo *nele(){
	return (nodo *)malloc(sizeof(nodo));
}
//Agregar
int agrega(nodo **inicio,nodo **final){
	nodo *nuevo;
	nuevo = nele();
	if(nuevo==NULL){
		printf("No hay memoria disponible!\n");
		exit(1);
	}else{
		printf("\n       Agregar:\n");
		printf("\n<---------------------->\n");
		printf("\n->");
		gets(nuevo->dato);
		gets(nuevo->dato);
		nuevo->sig = NULL;
		if(*inicio == NULL){
			*inicio = nuevo;
			*final = nuevo;
		}else{
			(*final)->sig=nuevo;
			*final=nuevo;
		}
	}
	return 0;
}
//Borrar
void borrar(nodo **inicio,nodo **final){
	int b=0;
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
			if(!strcmp((*inicio)->dato,nod)){
				aux3=*final;
				*final=*inicio=(*final)->sig;
				free(aux3);
				printf("\nCanción eliminada !!\n");
			}else b=2;
		}else{
			if(!strcmp((*inicio)->dato,nod)){//si es el primero
				aux3=*inicio;
				*inicio=(*inicio)->sig;
				aux3->sig=NULL;
				free(aux3);
				printf("\nDato borrado !!\n");
			}else{
				aux1=(*inicio)->sig;
				aux2=*inicio;
				do{
					if(!strcmp(aux1->dato,nod)){
						//si esta en medio
						aux2->sig=aux1->sig;
						aux1->sig=NULL;
						b=1;
						break;
					}else{
						if(aux1->sig==NULL){
							b=2;
							break;
						}else{
							aux1=aux1->sig;
							aux2=aux2->sig;
						}
					}
				}while(aux1->sig != NULL );
				if(b==1){
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
		printf("numero: %d \n Dato: %s",i+1,aux->dato);
		printf("\n<---------------------->\n");
		aux = aux->sig;
		i++;
	}
	if(i == 0)
		printf("\nNo hay datos!!\n");
	return i;
}
