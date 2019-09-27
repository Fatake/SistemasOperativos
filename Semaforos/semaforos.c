#include <sys/types.h>	/* para key_t */
#include <sys/ipc.h>
#include <sys/sem.h>
#define PERMISOS 0644

/*
 * creaSemaforo: abre o crea un semáforo
*/
int creaSemaforo (key_t clave, int valor_inicial ){
	int semid = semget(		/* Abre o crea un semáforo... */
		clave,				/* con una cierta clave */
		1,					/* con un solo elemento */
		IPC_CREAT|PERMISOS	/* lo crea (IPC_CREAT) con
							unos PERMISOS */
	);

	if ( semid < 0 )//Valores negativos si hubo errores
		return -1;

	/* Da el valor inicial al semáforo */
	semctl ( semid, 0, SETVAL, valor_inicial );
	return semid;
}

/*
 * abreSemaforo: Abre un semáforo que otro proceso ya creó
*/
int abreSemaforo (key_t clave){
	return semget(clave,1,0);
}

/*
 * Operador Decrementa
 */
void semDecre ( int semid ){
	struct sembuf opDec [] =
	{
	  0, -1, 0	/* Decrementa semval o bloquea si cero */
	};

	semop ( semid, opDec, 1 );
}
/*
 * Operador Incrementa
 */
void semIncre ( int semid ){
	struct sembuf opInc [] =
	{
	   0, 1, 0		/* Incrementa en 1 el semáforo */
	};
	semop ( semid, opInc, 1 );
}
/*
	struct sembuf {
	 unsigned short sem_num;	// número del semáforo dentro del conjunto
	 short		    sem_op;		// clase de operación
								// según sea >0, <0 o ==0
	 short			sem_flg;	// modificadores de operación
	};
*/
