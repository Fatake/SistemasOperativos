#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//Main
int main(int argc, char **argv){
	int fd;//Apuntador Archivo
	char c;
	//Open file
	fd = open(argv[1], O_RDONLY);
	if(fd != -1){
		while(read(fd,&c,sizeof(c)!=0))
			write(1,&c,sizeof(c));
		close(fd);
	}else
		printf("No se encontro archivo \n");
	return 0;
}
