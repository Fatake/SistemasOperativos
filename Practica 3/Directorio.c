#define True 1
#define False 0
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

//
void strmode(mode_t mode, char * buf) {
	const char chars[] = "rwxrwxrwx";
	for (size_t i = 0; i < 9; i++) {
		buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
	}
	buf[9] = '\0';
}

char * getUser(uid_t uid){
	struct passwd *pws;
	pws = getpwuid(uid);
		return pws->pw_name;
}

int parsetointLink(nlink_t *source){
	int buffer_size = sizeof(*source);
	char buffer[buffer_size];
	snprintf(&buffer[0], buffer_size, "%lu", (unsigned long)source);
	int val = atoi(buffer);
	return val;
}
int main(int argc, char **argv){
	DIR *dp;
	struct dirent *ep;
	struct stat sb;
	char buf[10];
	char leector[333];
	int pipefd[2];

	if (argc != 2) {
		printf("Directorio faltante en argumentos");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	//Procesos
	int cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0) {//Proceso Hijo
		close(pipefd[1]);
		while (read(pipefd[0], &leector, 1) > 0)
			write(1, &leector, 1);
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
	} else {//Proceso Padre

		close(pipefd[0]);

		dp = opendir(argv[1]);//Abre el directorio
		if (dp != NULL){
			while (ep = readdir (dp)){
				write(pipefd[1], "\0", strlen("\0"));
				lstat(ep->d_name, &sb);
				strmode(sb.st_mode,buf);

				write(pipefd[1], buf, strlen(buf));
				write(pipefd[1], " ", strlen(" "));
				write(pipefd[1], getUser(sb.st_uid), strlen(getUser(sb.st_uid)));
				write(pipefd[1], " ", strlen(" "));
				sprintf(leector, "", parsetointLink(sb.st_nlink));
				write(pipefd[1], leector, strlen(leector));
				write(pipefd[1], " ", strlen(" "));
				write(pipefd[1], (long long) sb.st_size, sizeof((long long) sb.st_size));
				write(pipefd[1], " ", strlen(" "));
				write(pipefd[1], ctime(&sb.st_ctime), strlen(ctime(&sb.st_ctime)));
				write(pipefd[1], " ", strlen(" "));
				write(pipefd[1], ep->d_name, sizeof(ep->d_name));
				write(pipefd[1], " ", strlen(" "));
				write(pipefd[1], "\n\n", strlen("\n\n"));

			}

			(void) closedir (dp);
		}else{
			perror ("No se pudo abrir el directorio\n\n");
			exit(EXIT_FAILURE);
		}

		close(pipefd[1]);
		wait(NULL);
	}

	return 0;
}
