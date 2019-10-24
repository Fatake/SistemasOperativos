#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define MAX_SEND_SIZE 800 //Para el tamaño máximo del mensaje

struct mymsgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

char* read_message(int qid, struct mymsgbuf *qbuf, long type);
void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text);

int main(int argc, char **argv){
	key_t key;
	int msgqueue_id;
	struct mymsgbuf qbuf;
	char  *saludoPadre, *saludoHijo;	
	
	saludoHijo = " >>> ¡Hola Hijo! <<<\n";
	
	key = ftok("-", 'm');
	
	/* Abrir la cola -- crearla si es necesario */
	if ((msgqueue_id  = msgget(key, IPC_CREAT|0660)) == -1) {
		perror("msgget");
		exit(1);
	}
	
	send_message(msgqueue_id, (struct mymsgbuf *)&qbuf, 3, saludoHijo);	
	
	printf (" ~~~ Soy el Padre ~~~\n");
	printf ("Mensaje de mi hijo: \n");
		
	saludoPadre = read_message(msgqueue_id, &qbuf, 1);
	printf ("%s", saludoPadre);	
	
	return 0;
}

char* read_message(int qid, struct mymsgbuf *qbuf, long type) {
	/* Leer mensaje de cola */
	//printf ("Leyendo mensaje...\n");
	qbuf->mtype = type;
	msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
	
	//printf ("Tipo: %ld Texto: %s\n", qbuf->mtype, qbuf->mtext);
	
	return qbuf->mtext;
}

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text) {
	/* Enviar mensaje a la cola */
	//printf ("Enviando mensaje...\n");
	qbuf->mtype = type;
	strcpy(qbuf->mtext, text);
	
	if ((msgsnd(qid, (struct msgbuf *)qbuf, strlen(qbuf->mtext)+1,0)) == -1) {
		perror ("msgsnd");
		exit(1);
	}
}
