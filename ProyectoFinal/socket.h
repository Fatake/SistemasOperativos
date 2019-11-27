#ifndef _socket_H_
#define _socket_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

//Constantes
#define BUFFSIZE 1
#define PUERTO 7200
#define DIRECION "127.0.0.1"

/*
 * Lee datos del socket. 
 * Recibe:
 * * int fd que es el numero del descriptor
 * * Char *Datos que se leerán
 * * int Longitud de los datos a leer
 * 
 * Retorna:
 * * numero de bytes leidos
 * * 0 si se cierra fichero 
 * * -1 si hay error
 */
int readSocket (int fd, char *Datos, int Longitud){
	int Leido = 0;
	int Aux = 0;

	/*
	 * Comprobacion de que los parametros de entrada son correctos
	 */
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	 * Mientras no hayamos leido todos los datos solicitados
	 */
	while (Leido < Longitud){
		/*
		 * Lectura del Socket
		 */
		Aux = read (fd, Datos + Leido, Longitud - Leido);
		if (Aux > 0){
			Leido = Leido + Aux;
		}else{
			/*
			 * Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
			 * los caracteres leidos hasta ese momento
			 */
			if (Aux == 0) 
				return Leido;
			if (Aux == -1){
				/*
				 * En caso de error.
				 * 
				 * El error EINTR se produce si ha habido alguna
				 * interrupcion del sistema antes de leer ningun dato. No
				 * es un error realmente.
				 * 
				 * El error EGAIN significa que el socket no esta disponible
				 * de momento, que lo intentemos dentro de un rato.
				 * Ambos errores se tratan con una espera de 100 microsegundos
				 * y se vuelve a intentar.
				 * 
				 * El resto de los posibles errores provocan que salgamos de 
				 * la funcion con error.
				 */
				switch (errno){
					case EINTR:
					case EAGAIN:
						usleep (100);
						break;
					default:
						return -1;
				}
			}
		}
	}
	return Leido;
}

/*
 * Escribe dato en el socket cliente
 * Recibe:
 * * int fd descriptor del archivos
 * * char *datos datos a escribir
 * * int longitud tamañio de los datos
 * 
 * Devuelve:
 * * numero de bytes escritos
 * * -1 si hay error.
 */
int WriteSocket (int fd, char *Datos, int Longitud){
	int Escrito = 0;
	int Aux = 0;

	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	while (Escrito < Longitud){
		/*
		 * Escribe en el Socket
		 */
		Aux = write (fd, Datos + Escrito, Longitud - Escrito);
		if (Aux > 0){
			Escrito = Escrito + Aux;
		}else{
			/*
			 * Si se ha cerrado el socket, devolvemos el numero de caracteres
			 * leidos.
			 * Si ha habido error, devolvemos -1
			 */
			if (Aux == 0)
				return Escrito;
			else
				return -1;
		}
	}

	return Escrito;
}

//<-----------------------------------Cliente------------------------------------>//

/*
 * Conecta con un servidor Unix, en la misma maquina.
 * Devuelve descriptor de socket si todo es correcto, -1 si hay error.
 */
int OpenSocketUnixCliente (char *Servicio){
	struct sockaddr_un Direccion;
	int Descriptor;

	strcpy (Direccion.sun_path, Servicio);
	Direccion.sun_family = AF_UNIX;

	Descriptor = socket (AF_UNIX, SOCK_STREAM, 0);
	if (Descriptor == -1)
		return -1;

	if (connect (Descriptor, (struct sockaddr *)&Direccion, strlen (Direccion.sun_path) + sizeof (Direccion.sun_family)) 
		== -1)
		return -1;

	return Descriptor;
}

/*
 * Conecta con un servidor remoto a traves de socket INET
 */
int OpenSocketINETCliente ( char *Host_Servidor, char *Servicio){
	struct sockaddr_in Direccion;
	struct servent *Puerto;
	struct hostent *Host;
	int Descriptor;

	Puerto = getservbyname (Servicio, "tcp");
	if (Puerto == NULL)
		return -1;

	Host = gethostbyname (Host_Servidor);
	if (Host == NULL)
		return -1;

	Direccion.sin_family = AF_INET;
	Direccion.sin_addr.s_addr = ((struct in_addr *)(Host->h_addr))->s_addr;
	Direccion.sin_port = Puerto->s_port;
	
	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1)
		return -1;

	if (connect ( Descriptor, (struct sockaddr *)&Direccion, sizeof (Direccion))
	 	== -1)
		return -1;

	return Descriptor;
}

//<----------------------------------Servidor----------------------------------->//

/*
 * Abre socket servidor UNIX(Local)
 * Recibe:
 * * char* servicio que se desea atender
 * 
 * Devuelve:
 * * descritor del socket servidor
 * * -1 en caso de error
 */
int OpenSocketUnix (char *Servicio){
	struct sockaddr_un Direccion;
	int Descriptor;

	Descriptor = socket (AF_UNIX, SOCK_STREAM, 0);
	if (Descriptor == -1)
	 	return -1;

	strcpy (Direccion.sun_path, Servicio);
	Direccion.sun_family = AF_UNIX;

	if (bind (
		Descriptor, (struct sockaddr *)&Direccion, 
		strlen (Direccion.sun_path) + sizeof (Direccion.sun_family)) == -1)
	{
		close (Descriptor);
		return -1;
	}

	if (listen (Descriptor, 1) == -1){
		close (Descriptor);
		return -1;
	}

	return Descriptor;
}

/*
 * Abre un socket servidor de tipo AF_INET
 * Puerto default 7200
 * Devuelve:
 * * el descripto del socket
 * * -1 si hay probleamas
 */
int OpenScoketINET (){
	struct sockaddr_in Direccion;
	int socketDescriptor, reuse;
	reuse = 1;

	/*
	 * se abre el socket
	 */
	socketDescriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor == -1)
	 	return -1;

	/*
	 * Se rellenan los campos de la estructura Direccion, necesaria
	 * para la llamada a la funcion bind()
	 */
	Direccion.sin_family = AF_INET;
	Direccion.sin_port = PUERTO;
	Direccion.sin_addr.s_addr = INADDR_ANY;

	/*
	 * Reusar El socket
	 * 
	 */
	if( setsockopt(socketDescriptor, 
		SOL_SOCKET, SO_REUSEADDR, 
		(char*)&reuse, sizeof(int)) == -1)
			return -1;

	if (bind (
			socketDescriptor, 
			(struct sockaddr *)&Direccion, 
			sizeof (Direccion)) == -1)
	{
		close (socketDescriptor);
		return -1;
	}

	return socketDescriptor;
}

/*
 * Escucha Peticiones de coneccion a un Socket
 * Recibe:
 * * int Socket Descriptor
 * * int cantidad de peticiones
 * 
 * Retorna:
 * * 0 si no hubo errores
 * * -1 si hay errores
 */
int escucharPeticones( int SocketDescriptor, int cantidad){
	if (listen (SocketDescriptor, cantidad) == -1){
		close (SocketDescriptor);
		return -1;
	}
	return 0;
}

/*
 * Accepta una coneccion a un Socket
 * Recibe:
 * * Descriptor del socket de servidor
 * 
 * Devuelve:
 * * numero de coneccion. 
 * * -1 si hay problemas.
 * Esta funcion vale para socket AF_INET o AF_UNIX.
 */
/*
int aceptarPeticion (int SocketDescriptor){
	struct sockaddr_storage clienteNuevo;
	unsigned int addres_size = sizeof(clienteNuevo);
	
	int coneccion = accept( SocketDescriptor, 
		(struct soackaddr*)&clienteNuevo,
		&addres_size);
	if (coneccion == -1){
		return -1;
	}
	return coneccion;
	//close(coneccion);
}*/

/*
 * Accepta una coneccion a un Socket
 * Recibe:
 * * Descriptor del socket de servidor
 * 
 * Devuelve:
 * * numero de coneccion. 
 * * -1 si hay problemas.
 * Esta funcion vale para socket AF_INET o AF_UNIX.
 */
int acceptarConeccion (int socketDescriptor){
	socklen_t Longitud_Cliente;
	struct sockaddr Cliente;
	int coneccion;

	Longitud_Cliente = sizeof (Cliente);
	coneccion = accept (socketDescriptor, &Cliente, &Longitud_Cliente);
	if (coneccion == -1)
		return -1;

	return coneccion;
}

#endif //Libreria de sockets
