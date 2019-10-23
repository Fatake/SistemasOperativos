#include <sys/types.h>
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

	semctl ( semid, 0, SETVAL, valor_inicial );
	return semid;
}
/*
 * Destruye semaforo
 */
int destruyeSemaforo (int semid, int semnum){
	if(!semctl ( semid, semnum, IPC_RMID ))
		return -1;
	return 1;
}
/*
 * Operador Decrementa
 */
void semDecre ( int semid ){
	/* Decrementa semval o bloquea si cero */
	struct sembuf opDec [] =
	{
	  0, -1, 0
	};

	if(semop ( semid, opDec, 1 ) < 0)
		exit(1);
}
/*
 * Operador Incrementa
 */
void semIncre ( int semid ){
	/* Incrementa en 1 el semáforo */
	struct sembuf opInc [] =
	{
	   0, 1, 0
	};
	if(semop ( semid, opInc, 1 ) < 0)
		exit(1);
}
/*
	struct sembuf {
	 unsigned short sem_num;	// número del semáforo dentro del conjunto
	 short		    sem_op;		// clase de operación
								// según sea >0, <0 o ==0
	 short			sem_flg;	// modificadores de operación
	};
*/
