#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
//ss
#define limpia() printf("\033[H\033[J")
//
//struct
//
typedef struct Musica{
	char nombre[50];
	struct Musica *sig;
}Mus;
//
//funciones
//
char *gets(char *s);
void wait();
int menu();
Mus *nele();
int agrega(Mus **inicio,Mus **final);
void borrar(Mus **inicio,Mus **final);
int mostrar(Mus *inicio);
void Rep(Mus *inicio);
//
//Main
//
int main(int argc, char **argv){
	int m=1,f=0,c;
	Mus *inicio,*final;
	inicio=final=NULL;
	//---------------------------------------------------------------------------------
	do{
		m=menu();
		switch(m){
			case 1:
				do{
					f=agrega(&inicio,&final);
				}while(f!=0);
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
			case 4:
				Rep(inicio);
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
//
//funciones
//
//menu
int menu(){
	int m=0;
	limpia();
	printf("\n       Programa que repoduce Musica\n");
	printf("\nSeleccione una opción:\n");
	printf("\n<------------------------->\n\n");
	printf("1)Agregar Canciones.\n");
	printf("2)Borrar Canciones.\n");
	printf("3)Mostrar Canciones.\n");
	printf("4)Reproducir una Canción.\n");
	printf("\n");
	printf("<---------------------->\n\n");
	printf("\n0)Salir. :(\n");
	printf("\n           Programa echo por \"Ruiz Lozano Paulo Cesar\"");
	printf("\n-> ");
	scanf("%d",&m);
	return m;
}
//recerva memoria
Mus *nele(){
	return (Mus *)malloc(sizeof(Mus));
}
//Espera
void wait(){
	int c;
	printf("\n<---------------------->\n");
	printf("Continuar...");
	scanf("%d",&c);
}
//agrega usuario
int agrega(Mus **inicio,Mus **final){
	int sal=0;
	Mus *nuevo;
	nuevo=nele();
	limpia();
	if(nuevo==NULL){//Segun literatura por si se acaba la memoria ram
		printf("No hay memoria disponible!\n");
		exit(1);
	}else{
		printf("\n       Agregar caciones:\n");
		printf("Para agregar caciones asegurece que sea en formato \"wav\"\n");
		printf("Recuerde tener las caciones en la misma carpeta de ecucion del programa\n");
		printf("\n<---------------------->\n");
		printf("\nIngrese nombre dela nueva cacion :\n->");
		gets(nuevo->nombre);
		gets(nuevo->nombre);
		nuevo->sig=NULL;
		if(*inicio==NULL){
			*inicio=nuevo;
			*final=nuevo;
		}else{
			(*final)->sig=nuevo;
			*final=nuevo;
		}
	}
	printf("\n<---------------------->\n");
	printf("\n Desea agregar mas caciones\n 0)para no");
	printf("\n cualquier tecla si\n->");
	scanf("%d",&sal);
	return sal;
}
//Muestra usuarios
int mostrar(Mus *inicio){
	Mus *aux; 
	int i=0;
	aux=inicio;
	printf("\n       Canciones:\n");
	printf("\n<---------------------->\n");
	while(aux!=NULL){
		printf("Cancion: \"%s\" numero: #%d:\n",aux->nombre,i+1);
		printf("\n<---------------------->\n");
		aux=aux->sig;
		i++;
	}
	if(i==0) printf("\nNo hay Canciones!!\n");
	return i;
}
//borrausuario
void borrar(Mus **inicio,Mus **final){
	int b=0;
	char nom[50];
	Mus *aux1,*aux2,*aux3;
	limpia();
	printf("			Borrar Musica\n");
	if(*inicio == NULL){
		printf("\n\n			Usted no ha agregado Canciones\n\n");
	}else{
		mostrar(*inicio);
		printf("\n<---------------------->\n");
		printf("Ingrese Nombre a eliminar: \n->");
		gets(nom);
		gets(nom);
		// proceso de eliminacion
		if(*final == *inicio){//si solo hay uno
			if(!strcmp((*inicio)->nombre,nom)){
				aux3=*final;
				*final=*inicio=(*final)->sig;
				free(aux3);
				printf("\nCanción eliminada !!\n");
			}else b=2;
		}else{
			if(!strcmp((*inicio)->nombre,nom)){//si es el primero
				aux3=*inicio;
				*inicio=(*inicio)->sig;
				aux3->sig=NULL;
				free(aux3);
				printf("\nCanción eliminada !!\n");
			}else{
				aux1=(*inicio)->sig;
				aux2=*inicio;
				do{
					if(!strcmp(aux1->nombre,nom)){
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
					printf("\nCanción eliminada !!\n");
				} 
			}
		}
		if(b==2) printf("\nLa canción no exite :(...\n");
	}
	wait();
}
//Reproducir cacion
void Rep(Mus *inicio){
	char can[50];
	int c;
	limpia();
	if((*inicio).nombre != NULL){
		mostrar(inicio);
		printf("\n<---------------------->\n");
		printf("Escriba nombre de cancion a reproducir:\n");
		printf("->");
		gets(can);
		gets(can);
		execlp("padsp","padsp","./rep",can,NULL);
	}else printf("No a agregado canciones :(\n");
	printf("\n<---------------------->\n");
	printf("Continuar...");
	scanf("%d",&c);
}
