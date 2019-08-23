#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#define limpia() printf("\033[H\033[J")
//
//Estrucctura PBC
//
typedef struct PBC{
	char user[150],PID[150], CPU[150], MEM[150], VSZ[150], RSS[150], TTY[150], STAT[150], START[150], TIME[150], COMMAND[150];
	struct PBC *sig;
}PBC;
//
//Prototipos de Funciones
//
//Funciones ordamentales
char *gets(char *s);
void pausa();
//Funciones de lista ligada
int borrar(PBC **inicio, PBC **final, PBC *nodo);
int agregar(PBC **inicio, PBC **final, char *line);
void mostrar(PBC **inicio, PBC **final);
int buscar(PBC **inicio, PBC *nodo);
//Funciones de manejo de memoria
PBC *new();
//Funciones que leer archivos
void leerArchivo(PBC **inicio, PBC **final);
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
		system("ps -aux > ps.txtx");
		//printf("Prceso hijo instrucciones ps -aux > ps.txt");
	}else{
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
int agregar(PBC **inicio, PBC **final, char *line){
	PBC *nuevo;
	int i = 0;
	char palabras[11][150];
	const char s[2] = " ";//Señal de separacion
	char *token; //Separador de Strings
	nuevo = new();

	token = strtok(line, s);

	while( token != NULL && i < 11) {
		strcpy(palabras[i++],token);
		token = strtok(NULL, s);
	}

	/*for(i=0;i<11;i++)
		printf("%s ",palabras[i]);
		* */


	//Reservar memoria de cada char
	/*
	nuevo->user    = (char*)malloc (sizeof(palabras[0]));
	nuevo->PID     = (char*)malloc (sizeof(palabras[1]));
	nuevo->CPU     = (char*)malloc (sizeof(palabras[2]));
	nuevo->MEM     = (char*)malloc (sizeof(palabras[3]));
	nuevo->VSZ     = (char*)malloc (sizeof(palabras[4]));
	nuevo->RSS     = (char*)malloc (sizeof(palabras[5]));
	nuevo->TTY     = (char*)malloc (sizeof(palabras[6]));
	nuevo->STAT    = (char*)malloc (sizeof(palabras[7]));
	nuevo->START   = (char*)malloc (sizeof(palabras[8]));
	nuevo->TIME    = (char*)malloc (sizeof(palabras[9]));
	nuevo->COMMAND = (char*)malloc (sizeof(palabras[10]));
	*/

	//Copiar los datos de la linea
	strcpy(nuevo->user,    palabras[0]);
	strcpy(nuevo->PID,     palabras[1]);
	strcpy(nuevo->CPU,     palabras[2]);
	strcpy(nuevo->MEM,     palabras[3]);
	strcpy(nuevo->VSZ,     palabras[4]);
	strcpy(nuevo->RSS,     palabras[5]);
	strcpy(nuevo->TTY,     palabras[6]);
	strcpy(nuevo->STAT,    palabras[7]);
	strcpy(nuevo->START,   palabras[8]);
	strcpy(nuevo->TIME,    palabras[9]);
	strcpy(nuevo->COMMAND, palabras[10]);

	nuevo->sig = NULL;
	//Preguntar si el nodo ya existe si no entondes lo de abajo
	if(*inicio == NULL){
		*inicio = nuevo;
		*final = nuevo;
	}else{
		(*final)->sig = nuevo;
		*final = nuevo;
	}
	return 1;
}
/*
 * Borrar
 * Retorna
 * 1 si se borro
 * 0 si no se borro
 */
int borrar(PBC **inicio, PBC **final, PBC *nodo){
	int b = 0;//Parte de los casos
	PBC *aux1,*aux2,*aux3;
	if(*inicio == NULL){
		return 0;// No hay elementos
	}else{
		// proceso de eliminacion
		if(*final == *inicio){//si solo hay uno
			if((*inicio) == nodo){
				aux3 = *final;
				*final = *inicio = (*final)->sig;
				free(aux3);
			}else
				b = 2;
		}else{
			if((*inicio) == nodo){//si es el primero
				aux3 = *inicio;
				*inicio = (*inicio)->sig;
				aux3->sig = NULL;
				free(aux3);
			}else{
				aux1 = (*inicio)->sig;
				aux2 = *inicio;
				do{
					if(aux1 == nodo){
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

	leerArchivo(inicio, final);
	aux = (*inicio);

	while(aux != NULL){
		printf("%s ",aux->user);
		printf("%s ",aux->PID);
		printf("%s ",aux->CPU);
		printf("%s ",aux->MEM);
		printf("%s ",aux->VSZ);
		printf("%s ",aux->RSS);
		printf("%s ",aux->TTY);
		printf("%s ",aux->STAT);
		printf("%s ",aux->START);
		printf("%s ",aux->TIME);
		printf("%s\n",aux->COMMAND);
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
void leerArchivo(PBC **inicio,PBC **final){
	FILE * fp;//Apuntador Archivo
	size_t len = 0;
	ssize_t read;
	char * line = NULL;//Linea a leer

	fp = fopen("ps.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1)
		agregar(inicio,final,line);

	fclose(fp);
	if (line)
		free(line);
}
