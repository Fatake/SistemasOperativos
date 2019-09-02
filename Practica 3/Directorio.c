#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>

//
void strmode(mode_t mode, char * buf) {
  const char chars[] = "rwxrwxrwx";
  for (size_t i = 0; i < 9; i++) {
    buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
  }
  buf[9] = '\0';
}

int main(int argc, char **argv){
	DIR *dp;
	struct dirent *ep;
	struct stat sb;
	char buf[10];
	dp = opendir(argv[1]);

	if (dp != NULL){
		while (ep = readdir (dp)){

			lstat(ep->d_name, &sb);

			strmode(sb.st_mode,buf);
			printf("%s ",buf);


			printf("%ld ", (long) sb.st_nlink);

			//printf(" UID=%ld   GID=%ld\n",
			//(long) sb.st_uid, (long) sb.st_gid);


			printf("%lld bytes ",
			(long long) sb.st_size);

			printf("%s ", ctime(&sb.st_ctime));
			puts (ep->d_name);
			printf("\n\n");

		}

		(void) closedir (dp);
	}else
	perror ("No se pudp abrir el directorio\n\n");

	return 0;
}
