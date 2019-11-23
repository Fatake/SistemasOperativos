#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
 
#define TBUF       2048
#define DirPadre  ".."



struct dirent **archivos;
struct stat atrib;
int total, k,r;
char buffer[1024];
uid_t UserID;
struct sockaddr_in server_addr,client_addr;
int sd, sc;
int size, val, expresion;
  int socket_id;
  int socket_acc;
  socklen_t s_lon;
  int r;
  struct sockaddr_in direccion;
  char hostName[255];
  struct hostent *host;
  char texto[255];


int num[1], res;

char  Dirinicial[TBUF] = ".";
 

  void imprimirArchivos(){ int k;

   if ( NULL == getcwd(Dirinicial,TBUF) )
   {
      perror("Error");
      return 0;
   }
   printf("\nDirectorio de trabajo = %s\n",Dirinicial);
  
   total = scandir (Dirinicial, &archivos, NULL, alphasort);
   printf("\nNumero de archivos DIRECTORIO ACTUAL= %d\n",total-2);

   for(k=0;k<total;k++)
   {
  	sprintf(buffer,"%s/%s",Dirinicial,archivos[k]->d_name);  
	stat(buffer,&atrib);
	printf("\t%s \t\t%0o \t\t%d  \n",archivos[k]->d_name, atrib.st_mode, 
	atrib.st_size);
   }
}

void chat(){
do{
 r =recv(sc, texto,255,0);
    if (r == 0)
      perror("Fin de la converzación");
    else {
      printf("Mensaje recibido: \n");
      write(1, texto, r);
      printf("\n");
}

}while(r!=0);
}

void hora(){
	struct timeval 	tv;
	struct tm* ptm;
	int aux;
	char time_string[40];
	//Obtiene hora y fecha del dia y se transforma en la estructura tm	
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	//la estructura tm creara un string con la informacion deseada
	strftime(time_string, sizeof(time_string), "%d/%m /%Y  %H:%M:%S" , ptm);

	printf("%s \n ", time_string);
	
	write(sc, &time_string, 40);
	
} 


void main(int argc, char *argv[])
{


sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
val = 1;
setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
bzero((char *)&server_addr, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = 4200;
bind(sd, &server_addr, sizeof(server_addr));
listen(sd, 5);
size = sizeof(client_addr);
printf("\n Conectado\n");
sc = accept(sd, (struct sockaddr *)&client_addr,&size);


read(sc, (char *) num, 2 *sizeof(int));
printf("la opcion es:%d\n",num[0]);
res=num[0];

switch(res){
		case 1:
				printf("Mis archivos son: \n");
				imprimirArchivos();
		break;
		case 4:
				printf("Iniciando converzacion: \n");
				chat();
		break;
		case 6:
				printf("La hora es: \n");
				hora();				

		break;

}
close (sd);

exit(0);
}