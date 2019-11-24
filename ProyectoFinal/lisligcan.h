#ifndef _lisligcan_H_
#define _lisligcan_H_

#include <stdio.h>
#include <stdlib.h>
/*
 * Estructura de la lista ligada Canciones
 */
typedef struct canciones{
	char nombre[50];
	struct canciones *sig;
}Canciones;

/*
 * Reserva Memoria para estructura canciones
 */
Canciones *nele(){
	return (Canciones *)malloc(sizeof(Canciones));
}

/*
 * Agrega canciones a la lista ligada
 */
int agregaCanciones(Canciones **inicio,Canciones **final, char* nombre){
	Canciones *nuevo;
	nuevo = nele();
	if(nuevo == NULL){
		printf("No hay memoria disponible!\n");
		exit(EXIT_FAILURE);
	}else{
		strcpy(nuevo->nombre,nombre);
		nuevo->sig = NULL;
		if(*inicio == NULL){
			*inicio = nuevo;
			*final = nuevo;
		}else{
			(*final)->sig = nuevo;
			*final = nuevo;
		}
	}
	return 1;
}

/*
 * Muestra todas las canciones de la lista ligada
 * retorna:
 * * numero de canciones
 * * -1 si no hay canciones
 */
int muestraCanciones(Canciones *inicio){
	Canciones *aux; 
	int i = 1;
	aux = inicio;
	if(aux == NULL){
		printf("No canciones en el servidor\n");
		return -1;
	}
	printf("\nCanciones:\n\n");
	while(aux != NULL){
		printf("%d) %s\n",(i++),aux->nombre);
		aux = aux->sig;
	}
	
	return i;
}

/*
 * Cuenta cuantos elementos hay en la lista ligada
 * retorna:
 * * numero de canciones
 * * -1 si no hay canciones
 */
int cantidadCanciones(Canciones *inicio){
	Canciones *aux; 
	int i = -1;
	aux = inicio;
	if(aux == NULL){
		printf("No canciones en el servidor\n");
		return i;
	}
	i = 0;
	while(aux != NULL){
		aux = aux->sig;
		i++;
	}
	return i;
}
/*
 * Busca el nombre de una cancion en la lista
 * Retorna:
 * * 1 si exite esa cancion en la lista
 * * 0 si no existe en la lista
 * * -1 si hay un error o la lista esta vacia
 */
int checaExistenciaCancion(Canciones *inicio, char * nombreCancion){
	Canciones *aux; 
	aux = inicio;
	if(aux == NULL)
		return -1;
	while(aux != NULL){
		if (!strcmp(aux->nombre,nombreCancion))
			return 1;

		aux = aux->sig;
	}
	return 0;
}

/*
 * Borra una cancion de la lista ligada
 * 
 */
int borraCancion(Canciones **inicio,Canciones **final, char* cancion){
	int b = 0;
	Canciones *aux1,*aux2,*aux3;

	if(*inicio == NULL){
		return -1;
	}else{
		if(*final == *inicio){
			if(!strcmp((*inicio)->nombre,cancion)){
				aux3 = *final;
				*final = *inicio = (*final)->sig;
				free(aux3);
			}else 
				b=2;
		}else{
			if(!strcmp((*inicio)->nombre,cancion)){
				aux3 = *inicio;
				*inicio = (*inicio)->sig;
				aux3->sig = NULL;
				free(aux3);
			}else{
				aux1 = (*inicio)->sig;
				aux2 = *inicio;
				do{
					if(!strcmp(aux1->nombre,cancion)){
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
		if(b == 2)
		 return -1;
	}
	return 0;
}

#endif //Lista Ligada Canciones Libreria
