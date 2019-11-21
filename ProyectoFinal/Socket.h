#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

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
 *	Abre socket servidor UNIX. Se le pasa el servicio que se desea atender y
 * que debe estar dado de alta en /etc/services. Deja el socket preparado
 * para aceptar conexiones de clientes.
 * Devuelve el descritor del socket servidor, que se debera pasar
 * a la funcion Acepta_Conexion_Cliente(). Devuelve -1 en caso de error
 */
int OpenSocketUnixServer (char *Servicio){
	struct sockaddr_un Direccion;
	int Descriptor;

	Descriptor = socket (AF_UNIX, SOCK_STREAM, 0);
	if (Descriptor == -1)
	 	return -1;

	/*
	 * Se rellenan en la estructura Direccion los datos necesarios para
	 * poder llamar a la funcion bind()
	 */
	strcpy (Direccion.sun_path, Servicio);
	Direccion.sun_family = AF_UNIX;

	if (bind (
		Descriptor, (struct sockaddr *)&Direccion, 
		strlen (Direccion.sun_path) + sizeof (Direccion.sun_family)) == -1)
	{
		close (Descriptor);
		return -1;
	}

	/*
	 * Avisamos al sistema que comience a atender peticiones de clientes.
	 */
	if (listen (Descriptor, 1) == -1){
		close (Descriptor);
		return -1;
	}

	return Descriptor;
}

/*
 * Se le pasa un socket de servidor y acepta en el una conexion de cliente.
 * devuelve el descriptor del socket del cliente o -1 si hay problemas.
 * Esta funcion vale para socket AF_INET o AF_UNIX.
 */
int AcceptConectSerCli (int Descriptor){
	socklen_t Longitud_Cliente;
	struct sockaddr Cliente;
	int Hijo;

	/*
	 * La llamada a la funcion accept requiere que el parametro 
	 * Longitud_Cliente contenga inicialmente el tamano de la
	 * estructura Cliente que se le pase. A la vuelta de la
	 * funcion, esta variable contiene la longitud de la informacion
	 * util devuelta en Cliente
	 */
	Longitud_Cliente = sizeof (Cliente);
	Hijo = accept (Descriptor, &Cliente, &Longitud_Cliente);
	if (Hijo == -1)
		return -1;

	/*
	 * Se devuelve el descriptor conectado al hijo
	 */
	return Hijo;
}

/*
 * Abre un socket servidor de tipo AF_INET. Devuelve el descriptor
 *	del socket o -1 si hay probleamas
 * Se pasa como parametro el nombre del servicio. Debe estar dado
 * de alta en el fichero /etc/services
 */
int OpenSockINETServer (char *Servicio){
	struct sockaddr_in Direccion;
	struct sockaddr Cliente;
	socklen_t Longitud_Cliente;
	struct servent *Puerto;
	int Descriptor;

	/*
	 * se abre el socket
	 */
	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1)
	 	return -1;

	/*
	 * Se obtiene el servicio del fichero /etc/services
	 */
	Puerto = getservbyname (Servicio, "tcp");
	if (Puerto == NULL)
		return -1;

	/*
	 * Se rellenan los campos de la estructura Direccion, necesaria
	 * para la llamada a la funcion bind()
	 */
	Direccion.sin_family = AF_INET;
	Direccion.sin_port = Puerto->s_port;
	Direccion.sin_addr.s_addr =INADDR_ANY;
	if (bind (
			Descriptor, 
			(struct sockaddr *)&Direccion, 
			sizeof (Direccion)) == -1)
	{
		close (Descriptor);
		return -1;
	}

	/*
	 * Se avisa al sistema que comience a atender llamadas de clientes
	 */
	if (listen (Descriptor, 1) == -1){
		close (Descriptor);
		return -1;
	}
	return Descriptor;
}
