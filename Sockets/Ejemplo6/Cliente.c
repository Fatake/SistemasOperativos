#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


int sd,op,r,ban1=1, ban2=1,ban3=1,ban4=1, ban5=1,var=5,res;
char texto[255];
struct sockaddr_in server_addr;
struct hostent *hp;
int num[1];
char buff[255];

void chat(){
		do{
			printf("Introduzca una palabra: ");
			scanf("%s", texto);
			r = send(sd, texto, strlen(texto),0);
			var--;
		}while(var!=0);	
}

void main(void){

	clock_t start = clock();
	struct timeval 	tv;
	struct tm* ptm;
	char time_string[40];
	char time_string2[40];
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	strftime(time_string, sizeof(time_string), "%d/%m /%Y  %H:%M:%S" , ptm);
	
	while(1){
	
		sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bzero((char *)&server_addr, sizeof(server_addr));
		hp = gethostbyname ("192.168.43.8");
		memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = 4200;

		printf("\nElija una opcion\n");
		printf(" 1. Listar los archivos del servidor\n ");
		printf("2. Copiar un archivo del servidor\n");
		printf(" 3. Enviar un archivo al servidor\n ");
		printf("4. Enviar una cadena al servidor\n");
		printf(" 5. Desconectar al servidor\n ");
		printf("6. Mostrar las estadisticas de la conexión\n");
		printf(" 0. Salir\n");
		scanf("%d",&op);

		switch(op){ 
			case 1:
				connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
				num[0]=1;
				write(sd, (char *) num, 1*sizeof(int));
				ban1=0;
			break;
				
			case 2:
				connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
				num[0]=2;
				write(sd, (char *) num, 1*sizeof(int));
				printf("Se copiara el archivo himno");
                FILE *ap;
                  read(sd, buff, 255);
				  ap = fopen("Himno.l", "w");
				  printf("%s",buff);
				  fprintf(ap, "%s", buff);
				  fclose(ap);
				ban2=0;
			break;

			case 3:
				connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
				num[0]=3;
				write(sd, (char *) num, 1*sizeof(int));
				printf("Se enviara el archivo cuento.l");
				  write(sd, "Habia una vez...", 21);
                  read(sd, buff, 9);
                  printf("%s", buff);
					printf("envio exitoso\n");
				ban3=0;
			break;
			
			case 4:	
				connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
				num[0]=4;
				write(sd, (char *) num, 1*sizeof(int)); 
				chat();
				ban4=0;
			break;
		    case 5: 
				connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
				num[0]=5;
				write(sd, (char *) num, 1*sizeof(int));
				read(sd, &time_string2, 40); 
				printf("%s \n ", time_string2);
				ban5=0;
			break;
			
			case 6: 
				printf("La hora en que se conecto el servidor fue: \n");
				printf("%s \n ", time_string);
				printf("\nTiempo transcurrido: %f segundos. \n", ((double)clock() - start) / CLOCKS_PER_SEC);
				printf("Operaciones realizadas hasta el momento:\n");
				if(ban1 == 0)
					printf("\nEl servidor listo sus archivos.\n");
				if(ban2 == 0)
					printf("Se copio un archivo del servidor.\n ");
				if(ban3 == 0)
					printf("Se envio un archivo al servidor.\n");
				if(ban4 == 0)
					printf("Se envi­o mensajes al servidor.\n ");
				if(ban5 == 0)
					printf("Se mostro la hora de salida del servidor y procecededio a desconectarse.\n ");
				printf("Se mostraron las estadisticas.\n");
			break;
			
			case 0:
				exit(0);
			break;

			dafault:
				printf("Opcion invalida");
			break;


	}

	close (sd);
	}
exit(0);
}
