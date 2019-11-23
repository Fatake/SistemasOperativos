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

typedef struct Cabecera{
    int identificador;
    int longitud; /* Longitud del mensaje, en bytes */
} Cabecera;
typedef struct MensajeDameFecha{
    long fechaHora;
} MensajePedirFecha;
int sd,op,r;
char texto[255];
struct sockaddr_in server_addr;
struct hostent *hp;
int num[1], res;
int var=5;
int ban1=1, ban2=1,ban3=1,ban4=1, ban5=1, ban6=1;
void chat(){
		do{
	printf("Introduzca una palabra: ");
			scanf("%s", texto);
			r = send(sd, texto, strlen(texto),0);
var=var-1;	
			/*if (r == -1)
      			perror("Error enviando mensaje");*/
}while(var!=0);	

			

}


   

void main(void)
{

clock_t start = clock();
struct timeval 	tv;
	struct tm* ptm;
	char time_string[40];
	//Obtiene hora y fecha del dia y se transforma en la estructura tm	
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	//la estructura tm creara un string con la informacion deseada
	strftime(time_string, sizeof(time_string), "%d/%m /%Y  %H:%M:%S" , ptm);
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero((char *)&server_addr, sizeof(server_addr));
	hp = gethostbyname ("172.31.15.184");
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = 4200;
// se establece la conexión

	printf("Elija una opcion");
	printf("\n 1. Listar los archivos del servidor \n ");
	printf("4. Enviar una cadena \n");
	scanf("%d",&op);

	switch(op){ 
		case 1:
			connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
			num[0]=1;
			write(sd, (char *) num, 1*sizeof(int));
			
			ban1=0;
		break;
		
		case 4:	
			connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
			num[0]=4;
			write(sd, (char *) num, 1*sizeof(int)); 
			chat();
		ban4=0;
		break;
		case 5: 
		printf("La hora en que se conectó el servidor fue: \n");
		//printf("%s \n ", time_string);
 		printf("\nTiempo transcurrido: %f", ((double)clock() - start) / CLOCKS_PER_SEC);
		printf("\nOperaciones realizadas hasta el momento: \n");
		if(ban1==0)
		printf("\nEl servidor mostró sus archivos \n");
		if(ban2==0)
		printf("\nSe copío un archivo del sistema\n ");
		if(ban3==0)
		printf("\nSe envío un archivo al sistema\n");
		if(ban4==0)
		printf("\nSe envío mensaje al sistema\n ");
		if(ban5==0)
		printf("\nSe mostró la hora de salida del servidor\n ");
		break;

		case 6: 
			connect(sd, (struct sockaddr *) &server_addr,sizeof(server_addr));
			num[0]=6;
			write(sd, (char *) num, 1*sizeof(int));
			read(sd, &time_string, 40); // recibe la respuesta}
		         printf("%s \n ", time_string);
		break;
}

close (sd);
exit(0);
}
