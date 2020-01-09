#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
 
#define SIZE 512
 
int main( int argc, char **argv )
{
  pid_t pid;
  int p[2], readbytes;
  char buffer[SIZE];
 
  pipe( p );
 
  if ( (pid=fork()) == 0 )
  { // hijo
    close( p[1] ); /* cerramos el lado de escritura del pipe */
 
    while( (readbytes=read( p[0], buffer, SIZE )) > 0)
      write( 1, buffer, readbytes );
 
    close( p[0] );
  }
  else
  { // padre
    close( p[0] ); /* cerramos el lado de lectura del pipe */
 
    strcpy( buffer, "Esto llega a traves de la tuberia\n" );
    write( p[1], buffer, strlen( buffer ) );
 
    close( p[1] );
  }
  waitpid( pid, NULL, 0 );
  exit( 0 );
}