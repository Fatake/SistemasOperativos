#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define limpia() printf("\033[H\033[J")
//
//Estrucctura PBC
//
typedef struct PBC{
	char *user,*PID, *CPU, *MEM, *VSZ, *RSS, *TTY, *STAT, *START, *TIME, *COMMAND;
	struct PBC *sig;
}PBC;
//
//Prototipos de Funciones
//
//Funciones ordamentales
char *gets(char *s);
void pausa();
//Funciones de lista ligada
int borrar(PBC **inicio, PBC **final, PBC nodo);
int agregar(PBC **inicio, PBC **final, PBC nodo);
void mostrar(PBC **inicio, PBC **final);
int buscar(PBC **inicio, PBC nodo);
//Funciones de manejo de memoria
PBC *new();
//Funciones que leer archivos
int leerArchivo(PBC **inicio, PBC **final, char * nombreArchivo);
//
//Main
//
int main(int argc, char **argv){
	int pid;//Control de procesos
	PBC *inicio,*final;
	inicio = final = NULL;

	printf("\tProcesos:\n");
	printf("\n<------------------------>\n");
	if((pid = fork()) == 0){
		//while( time() % 5 == 0)
		printf("Prceso hijo instrucciones ps -aux > ps.txt");
	}else{
		printf("Ejeccucion normal del programa\n");
		mostrar(&inicio, &final);
	/*
	 * Contador de Tiempo quer ejecute mostrar cada 5 s
	 * while(time() % 5 == 0)
	 * 	ejecuta mostar()
	 */
	}
	return 0;
}
//recerva memoria
PBC *new(){
	return (PBC *)malloc(sizeof(PBC));
}
/**
 * Agregar
 * Retorna:
 * 1 si lo agrego
 * 0 si no lo agrego
 */
int agregar(PBC **inicio, PBC **final, PBC nodo){
	//Preguntar si el nodo ya existe si no entondes lo de abajo
	if(*inicio == NULL){
		*inicio = &nodo;
		*final = &nodo;
	}else{
		(*final)->sig = &nodo;
		*final = &nodo;
	}
	return 1;
}
/*
 * Borrar
 * Retorna
 * 1 si se borro
 * 0 si no se borro
 */
int borrar(PBC **inicio, PBC **final, PBC nodo){
	int b = 0;//Parte de los casos
	PBC *aux1,*aux2,*aux3;
	if(*inicio == NULL){
		return 0;// No hay elementos
	}else{
		// proceso de eliminacion
		if(*final == *inicio){//si solo hay uno
			if((*inicio) == &nodo){
				aux3 = *final;
				*final = *inicio = (*final)->sig;
				free(aux3);
			}else
				b = 2;
		}else{
			if((*inicio) == &nodo){//si es el primero
				aux3 = *inicio;
				*inicio = (*inicio)->sig;
				aux3->sig = NULL;
				free(aux3);
			}else{
				aux1 = (*inicio)->sig;
				aux2 = *inicio;
				do{
					if(aux1 == &nodo){
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
				if(b == 1)
					free(aux1);
			}
		}
		if(b==2)
			return 0;
	}
	return 1;
}
//Mostrar
void mostrar(PBC **inicio, PBC **final){
	PBC *aux;
	int i = 0;
	aux = (*inicio);

	leerArchivo(inicio, final, "ps.txt");

	while(aux != NULL){
		printf("Proceso: %d \n %s",i+1,aux->PID);
		aux = aux->sig;
		i++;
	}
	if(i == 0)
		printf("\nNo hay datos!!\n");
}
//
void pausa(){
	printf("\n<---------------------->\n");
	printf("Continuar...");
	getchar();
	getchar();
}
/*
 * Lee Archivo
 */
int leerArchivo(PBC **inicio,PBC **final,char * nombreArchivo){
	FILE * fp;//Apuntador Archivo
	size_t len = 0;
	ssize_t read;
	char * line = NULL;//Line a leer
	int contadorPaginas = 1;

	fp = fopen(nombreArchivo, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		//Token strktoken
		if(contadorPaginas != 1){
			/*
			 * La primera linea del archivo ps.txt tiene solo las
			 * cabezeras, por eso no leemos esa linea

			PBC *nuevo;
			nuevo = new();
			//Reservar memoria de cada char
			nuevo->user = (char*)malloc (sizeof(Caracter leido del usuario));
			nuevo->PID = (char*)malloc (sizeof());
			nuevo->CPU = (char*)malloc (sizeof());
			nuevo->MEM = (char*)malloc (sizeof());
			nuevo->VSZ = (char*)malloc (sizeof());
			nuevo->RSS = (char*)malloc (sizeof());
			nuevo->TTY = (char*)malloc (sizeof());
			nuevo->STAT = (char*)malloc (sizeof());
			nuevo->START = (char*)malloc (sizeof());
			nuevo->TIME = (char*)malloc (sizeof());
			nuevo->COMMAND = (char*)malloc (sizeof());

			//Copiar los datos de la linea
			strcpy(nuevo->user, user);
			strcpy(nuevo->PID, PID);
			strcpy(nuevo->CPU, CPU);
			strcpy(nuevo->MEM, MEM);
			strcpy(nuevo->VSZ, VSZ);
			strcpy(nuevo->RSS, RSS);
			strcpy(nuevo->TTY, TTY);
			strcpy(nuevo->STAT, STAT);
			strcpy(nuevo->START, START);
			strcpy(nuevo->TIME, TIME);
			strcpy(nuevo->COMMAND, COMMAND);

			nuevo->sig = NULL;
			agregar(inicio,final,nuevo);
			*/
		}
		contadorPaginas ++;
	}

	fclose(fp);
	if (line)
		free(line);

	return contadorPaginas;
}
