#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/msg.h>
#include <stdlib.h>
#define PERMISOS 0644
#define TAMANIO_MAXIMO_MENSAJE 150

typedef struct mi_mensaje{
	long mtype;
	char mensaje[TAMANIO_MAXIMO_MENSAJE];
}miMensaje;

/*
 * Crea una memoria compartida dependiedo del tamaño dado
 * Retorna un -1 si no se completo la accion
 * Rterona un numero positivo si se completo <shmid>
 */
int createSHM(int size){
	return shmget( 0,sizeof(size),IPC_PRIVATE|IPC_CREAT|PERMISOS );
}

/*
 * Crea una memoria compartida dependiedo del tamanio dado con unos permisos
 * Retorna un -1 si no se completo la accion
 * Rterona un numero positivo si se completo <shmid>
 */
int createSHMPerm(int size, int perm){
	return shmget( 0,sizeof(size),IPC_PRIVATE|IPC_CREAT|perm );
}

/*
 * Elimina una memoria compartida con un shmid de parametro
 */
int deleteSHM(int shmid){
	return shmctl( shmid, IPC_RMID, NULL );
}
/*
 * Borra un mensaje
 */
int deleteMSG( int mensajeID ){
	return msgctl( mensajeID, IPC_RMID, NULL );
}
/*
 * Leer Mensaje
 */
char* leerMensaje(int msgid, miMensaje *mensaje, long type) {
	mensaje->mtype = type;
	msgrcv(msgid, (miMensaje *)mensaje, TAMANIO_MAXIMO_MENSAJE, type, 0);
	
	return mensaje->mensaje;
}
/*
 * Enviar Mensaje
 */
int enviarMensaje(int msgid, miMensaje *mensaje, long type, char *text) {
	mensaje->mtype = type;
	strcpy( mensaje->mensaje, text);
	
	if ( (msgsnd(msgid, (miMensaje *)mensaje, strlen( mensaje->mensaje )+1,0)) == -1)
		return -1;

	return 1;
}

/*
 * Crea un mensaje
 */
int createMSG(char *textoMensaje){
	miMensaje mensajeNuevo;
	int msgid;
	key_t msgkey;

	//Obtiene una llave
	msgkey = ftok("/bin/ls",33);
	if(msgkey == (key_t)-1){
		printf("ERROR AL OBTENER KEY DEL MENSAJE\n");
		return -1;
	}
	//Crea a cola de mensajes
	msgid = msgget( msgkey, PERMISOS|IPC_CREAT );
	if(msgid == -1){
		printf("ERROR AL OBTENER ID DE MENSAJE\n");
		return -1;
	}

	if(enviarMensaje( msgid, (miMensaje *) &mensajeNuevo, 3, textoMensaje ) == -1){
		perror ("msgsnd");
		exit(1);
	}

	return msgid;
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
