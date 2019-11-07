#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ipcfunctions.h"

/*
 * Main
 */
int main(int argc, char **argv){
	miMensaje auxmsg;
	int aux,i;
	char auxc[50], *auxc2;

	if(argc >= 4){
		//
		//Memoria
		//
		if( !strcmp( argv[1],"-m" ) ){
			if(!strcmp(argv[2],"-c")){//Crear Memoria
				if (argc > 5){
					aux = createSHMPerm( atoi(argv[3]), atoi(argv[5]) );
					if(aux > 0){
						printf("Memoria Compartida Creada con el shmid: %d ",aux);
						printf("con los Permisos %d\n",atoi(argv[5]));
					}else
						printf("Error al Crear la Memoria Compartida");
				}else{
					aux = createSHM(atoi(argv[3]));
					if(aux > 0)
						printf("Memoria Compartida Creada con el shmid: %d",aux);
					else
						printf("Error al Crear la Memoria Compartida");
				}
				
			}else if(!strcmp(argv[2],"-d")){//Borrar
				if(!deleteSHM(atoi(argv[3])))
					printf("Memoria con shmid %d Borrada\n",atoi(argv[3]));
				else
					printf("Error al Borrar la Memoria");
			}
		//
		//Semaforos
		//
		}else if(!strcmp(argv[1],"-s")){
			if(!strcmp(argv[2],"-c")){//Crear Semáforo
				if( argc > 4 && atoi(argv[3] ) > 0 ){
					for( i = 1; i <= atoi(argv[3]); i++){
						if( atoi(argv[4]) < 0 ){
							printf("No se pueden crear semaforo con valor negativo u.u\n");
							break;
						}else{
							aux = creaSemaforo(0,atoi(argv[4]));
							if(aux > 0)
								printf("Semaforo Creado semid: %d valor: %d\n",aux,atoi(argv[4]));
							else
								printf("Error al Crear el Semaforo %d\n",i);
						}
					}
				}else if(atoi(argv[3]) > 0){
					for( i = 1; i <= atoi(argv[3]); i++){
						aux = creaSemaforo(0,0);
						if(aux > 0)
							printf("Semaforo Creado semid: %d valor: 0\n",aux);
						else
							printf("Error al Crear el Semaforo %d\n",i);
					}
				}else
					printf("Error, #sem debe ser mayor que 0");
			}else if( !strcmp(argv[2],"-d") ){//Borrar
				if(destruyeSemaforo(atoi(argv[3]),0) != -1)
					printf("Semid %d Borrado\n",atoi(argv[3]));
				else
					printf("Error al Borrar El semaforo con semid: %d",atoi(argv[3]));
			}
		//
		//Mensajes
		//
		}else if( !strcmp(argv[1],"-w" )){

			if( !strcmp(argv[2],"-c" )){//Crear
				strncpy( auxc,argv[3],49 );
				auxc[49] = '\0';
				aux = createMSG(auxc);
				if( aux >= 0)
					printf("Mensaje creado\n msgid: %d \nMensaje: %s",aux,auxc);
				else
					printf("Error al Crear el mensaje u.u");
	
			}else if( !strcmp(argv[2],"-d" )){//Borrar
				if( !deleteMSG(atoi(argv[3])) )
					printf("Mensaje con msgid %d Borrado\n",atoi(argv[3]));
				else
					printf("Error al Borrar el Mensaje %d",atoi(argv[3]));
	
			}else if ( !strcmp(argv[2],"-r") ){//Leer
				if ( argc > 3 ){
					printf("Empezando a leer...\n");
					auxc2 = leerMensaje( atoi(argv[3]), &auxmsg, 3 );
					printf ("Mensaje: \n%s\n", auxc2);	
				}else
					printf("Error al leer mensaje u.u\n");
				
			}
			
		}else
			printf("Parametro no encontrado\nUtilice --help Para obtener ayuda");
	//
	//--Help
	//
	}else{
		if(argc < 4){
			if(argc > 1 && !strcmp(argv[1],"--help")){
				printf("\tPrograma que crear recursos IPC \n\n");
				printf("Parametros admitidos:\n\n");

				printf("-m <Memoria Comparitda> -c <Crear> <Int:Tamanio>\n");
				printf("-m <Memoria Comparitda> -c <Crear> <Int:Tamanio> -p [<permisos> <Int:Permisos>]\n");
				printf("-m <Memoria Comparitda> -d <Borrar> <Int:shmid>\n\n");

				printf("-s <Semaforo> -c <Crear> <Int:Cantidad> <Int:Valor Inicial>\n");
				printf("-s <Semaforo> -d <Borrar> <Int:semid>\n");
				printf("-s -c <Int:Cantidad> si no se especifica el 4rto parametro el valor defecto es 0\n\n");

				printf("-w <Mensajes> -c <Crear> <Char:Mensaje> **\"Cadena entre comillas\"\n");
				printf("-w <Mensajes> -d <Borrar> <Int: msgid>\n");
				printf("-w <Mensajes> -r <Leere> <Int: msgid>\n");

				printf("\n\nEjemplo:\n");
				printf("./ipc -s -c 3 0\n");
				printf("Creara 3 semaforos con valor 0\n");
			}else
				printf("Parametro no encontrado\nUtilice --help Para obtener ayuda");
		}
	}
	printf("\n");
	return 0;
}
