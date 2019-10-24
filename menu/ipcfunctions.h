#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/msg.h>
#include <stdlib.h>
#define PERMISOS 0644

typedef struct my_msg{
	long ID;
	int inf;
	char Msg[10];
}mymsg;

/*
 * Crea una memoria compartida dependiedo del tamaño dado
 * Retorna un -1 si no se completo la accion
 * Rterona un numero positivo si se completo <shmid>
 */
int createSHM(int size){
	return shmget( 0,sizeof(size),IPC_PRIVATE|IPC_CREAT|PERMISOS );
}

/*
 * Elimina una memoria compartida con un shmid de parametro
 */
int deleteSHM(int shmid){
	return shmctl( shmid, IPC_RMID, NULL );
}

/*
 * Crea un mensaje
 */
int createMSG(char x[]){
	mymsg msg;

	key_t msgkey = ftok("/bin/ls",33);
	if(msgkey == (key_t)-1){
		printf("ERROR AL OBTENER KEY DEL MENSAJE\n");
		return -1;
	}

	int msgid = msgget( msgkey,0600|IPC_CREAT );
	if(msgid == -1){
		printf("ERROR AL OBTENER ID DE MENSAJE\n");
		return -1;
	}
	msg.ID = 1;
	msg.inf = 29;
	strcpy(msg.Msg,x);

	return msgid;
}
/*
 * Borra un mensaje
 */
int deleteMSG(int msgid){
	return msgctl( msgid,IPC_RMID,NULL );
}
/*
 * Lee un mensaje
 */
char * read_message( int qid, long type, mymsg *qbuf ){
	int	result, length;

	/* The length is essentially the size of the structure minus siz) */eof(mtype
	length = sizeof(struct mymsgbuf) - sizeof(long);

	if((result = msgrcv( qid, qbuf, length, type,  0)) == -1){
		return(NULL);
	}

	return(result);
}

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
	return semctl ( semid, semnum, IPC_RMID );
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

	semop ( semid, opDec, 1 );
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
	semop ( semid, opInc, 1 );
}

