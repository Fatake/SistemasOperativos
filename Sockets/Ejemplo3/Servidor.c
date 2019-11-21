/*********************************************************************/
/* SERVIDOR */
/*********************************************************************/
/* Se muestra el uso de algunas funciones para manejo de sockets
*/
#include <stdio.h>
#include <sys/socket.h> /* **** */
#include <sys/stat.h>
#include <sys/types.h> /* **** */
#include <netinet/in.h>
#include <signal.h>
#define PUERTO 55556
/* s es el descriptor del socket */
/* cs es el descriptor del socket que regresa la funcion accept */
int s;
int cs;

int server();
//
//Main
//
int main(){
    struct sockaddr_in local;
    /* Se crea el socket */
    s = socket(AF_INET,SOCK_STREAM,0);
    /* Se asigna a los diferentes campos de la variable local para
    que el servidor pueda recibir solicitud de conexiones */

    local.sin_family = AF_INET;
    local.sin_port = htons(PUERTO);
    local.sin_addr.s_addr = INADDR_ANY;
    /* Se amarra el socket a una direccion */

    bind (s, (struct sockaddr *) &local, sizeof(local));
    /* Se crea la cola de espera */
    listen (s, 5);
    /* Ciclo infinito de escucha-respuesta */
    while (1){
        /* Se acepta una nueva solicitud de conexi&oacute;n */
        cs = accept (s, (struct sockaddr *) 0, (int *) 0);
        server (); /* Funcion que atiende al cliente */
        close (cs); /* Termina el proceso padre-arranque */
    }
    return 1;
}
    
/* 
 * Esta es la funcion que atiende al proceso cliente
 */
int server(){
    int t;
    char buff[255];
    FILE *ap;
    read (cs, buff, 255);
    ap = fopen("serv3.l", "w");
    fprintf(ap, "%s", buff);
    fclose(ap);
    return 1;
}
