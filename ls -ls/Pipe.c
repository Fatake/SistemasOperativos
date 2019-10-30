#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>

typedef struct {
  char nombre[256];
  unsigned long inodo;
  int  tipo;
  char propietario[32];
  long  tamano;
  int  fecha[3];
  int  hora[2];
  unsigned int permisos;
} DatosArchivo;

void mostrarArchivo(DatosArchivo *datos, const char *rutaCompleta);

int main(int argc, char **argv) {  
  int fd[2];
  DatosArchivo datos;
  DIR *directorio;
  struct dirent *archivo;
  struct stat st;
  struct passwd *usuario;
  struct tm *fecha;
  int longitud, inicioArchivo;
  char rutaCompleta[1000];

  if (argc > 1) {
    pipe(fd);
  
    if (fork() == 0) {
      close(fd[0]);

      directorio = opendir(argv[1]);

      if (directorio != NULL) {
        strcpy(rutaCompleta,argv[1]);
        longitud = inicioArchivo = strlen(rutaCompleta);
        if(rutaCompleta[longitud-1] != '/') {
          rutaCompleta[longitud] = '/';
          inicioArchivo ++;
        }

        while (archivo = readdir(directorio)) {
          strcpy(rutaCompleta + inicioArchivo, archivo->d_name);

          stat(rutaCompleta, &st); 
          usuario = getpwuid(st.st_uid);
          fecha = localtime(&st.st_mtime);          
          
          strcpy(datos.nombre, archivo->d_name);
          datos.inodo = archivo->d_ino;
          datos.tipo = archivo->d_type;
          strcpy(datos.propietario, usuario->pw_name);
          datos.tamano = st.st_size;
          datos.fecha[0] = fecha->tm_mday;
          datos.fecha[1] = fecha->tm_mon + 1;
          datos.fecha[2] = fecha->tm_year + 1900;
          datos.hora[0] = fecha->tm_hour;
          datos.hora[1] = fecha->tm_min;
          datos.permisos = st.st_mode;
          
          write(fd[1], &datos, sizeof(DatosArchivo));
        }

        close(fd[1]);
        closedir(directorio);
      } else {
        printf("Error: No se pudo abrir el directorio.\n");
      }
    } else {
      close(fd[1]);
      printf("%8s  %-11s %-10s %7s %-10s %-5s %-9s  %-12s\n", "INODO", "TIPO", "PROPIETARIO", "TAMAÑO", "FECHA", "HORA", "PERMISOS", "NOMBRE" );
      
      while (read(fd[0], &datos, sizeof(DatosArchivo)))
        mostrarArchivo(&datos,rutaCompleta);
    }
  } else {
    printf("Error: No se proporcionó un directorio para mostrar.\n");
  }
}

void imprimirPermisos(unsigned int modo) {
  static int banderas[] = { 
    S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH
   };
  int i;

  for (i = 0; i < 9; i++) {
    if ((modo & banderas[i]) != 0) {
      switch (i % 3)
      {
      case 0:
        putchar('r');
        break;
      case 1:
        putchar('w');
        break;
      case 2:
        putchar('x');
        break;
      }
    } else {
      putchar('-');
    }
  }
}

void mostrarArchivo(DatosArchivo *datos, const char *rutaCompleta) {

  // Imprimir el inodo
  printf("%8d  ", datos->inodo);

  // Imprimir el tipo
  if (datos->tipo == 4)
    printf("%-11s ", "Carpeta");
  else if (datos->tipo == 8)
    printf("%-11s ", "Archivo");
  else
    printf("%-11s ", "Desconocido");
  
  // Imprimir el propietario
  printf("%-10s ", datos->propietario);

  //Imprimir el tamaño 
  printf("%7d " , datos->tamano);

  // Imprimir la fecha
  printf("%02d/%02d/%4d ", datos->fecha[0], datos->fecha[1], datos->fecha[2]);

  // Imprimir la hora
  printf("%02d:%02d ", datos->hora[0], datos->hora[1]);

  // Imprimir permisos
  imprimirPermisos(datos->permisos);
  printf("  ");

  // Imprimir nombre
  printf("%-12s ", datos->nombre);

  printf("\n");  
}
