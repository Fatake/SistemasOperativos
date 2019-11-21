/*********************************************************************/
/* CLIENTE */
/*********************************************************************/
/* Este programa se conecta al servidor. */
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#define PUERTO 55556

int main(){
    char buff[255]; /* Buffer para la recepcion */
    int s; /* Descriptor del socket */
    struct sockaddr_in server_info; /* Estructura para almacenar la
    informacion del servidor */
    struct hostent *host; /* Estructura para obtener la
    direccion IP del servidor */

    s=socket (AF_INET, SOCK_STREAM,0); /* Creacion del socket */
    host=gethostbyname("148.228.20.86"); /* Obtencion de la
    dir. IP del servidor */
    bcopy (host->h_addr, &server_info.sin_addr,host->h_length);
    server_info.sin_family=AF_INET;
    server_info.sin_port=htons (PUERTO);
    /* Se establece la conexi&oacute;n del cliente y el servidor */
    connect (s, (struct sockaddr *) &server_info, sizeof(server_info));
    /* Escritura del mensaje al servidor */
    write (s, "Hola Bety como estas!", 21);
    /* Se imprime el buffer en la salida estandar */
    close (s); /* Se cierra el socket */
    return 1;
}
/* Termina el programa cliente */