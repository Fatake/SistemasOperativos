#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

void crearMemoriaCompartida();
void escribirMemoriaCompartida();
void leerMemoriaCompartida();
void liberarMemoriaCompartida();

void operacionesMemoriaCompartida() {
  int opcion;

  do {
    limpiarPantalla();
    printf("=== Operaciones de memoria compartida ===\n");
    printf("1. Crear bloque de memoria compartida\n");
    printf("2. Escribir en memoria compartida\n");
    printf("3. Leer memoria compartida\n");
    printf("4. Liberar bloque de memoria compartida\n");
    printf("5. Regresar\n");

    switch (obtenerOpcion()) {
      case 1:
        crearMemoriaCompartida();
        break;

      case 2:
        escribirMemoriaCompartida();
        break;

      case 3:
        leerMemoriaCompartida();
        break;

      case 4:
        liberarMemoriaCompartida();
        break;

      case 5:
        return;

      default:
        printf("\nNo se reconoció la opción.\n");
        esperarEnter();
    }
  } while (1);
}

void crearMemoriaCompartida() {
  int tamano, permisos, shmid;

  printf("Introduzca el tamaño del bloque de memoria compartida (en bytes): ");
  scanf("%d%*c", &tamano);

  printf("Ingrese los permisos del semáforo (en octal): ");
  scanf("%o%*c", &permisos);

  shmid = shmget(IPC_PRIVATE, tamano, IPC_CREAT| permisos);

  if (shmid >= 0) 
    printf("Se creó exitosamente el bloque de memoria compartida con shmid %d.\n", shmid);
  else 
    printf("Ocurrió un error al crear el bloque de memoria compartida.\n");
  
  esperarEnter();
}

void escribirMemoriaCompartida() {
  int shmid;
  struct shmid_ds info;
  char *memoria;

  printf("Introduzca el shmid de la memoria compartida: ");
  scanf("%d%*c", &shmid);

  if (shmctl(shmid, IPC_STAT, &info) < 0) {
    printf("No existe un bloque de memoria con el shmid especificado.\n");
    esperarEnter();
    return;
  } 

  memoria = shmat(shmid, 0, 0);

  printf("Introduzca una cadena que se escribirá en la memoria compartida (max %d caracteres): ", info.shm_segsz - 1);
  fgets(memoria, info.shm_segsz, stdin);

  esperarEnter();
}

void leerMemoriaCompartida() {
  int shmid, i;
  struct shmid_ds info;
  char *memoria;

  printf("Introduzca el shmid de la memoria compartida: ");
  scanf("%d%*c", &shmid);

  if (shmctl(shmid, IPC_STAT, &info) < 0) {
    printf("No existe un bloque de memoria con el shmid especificado.\n");
    esperarEnter();
    return;
  } 

  memoria = shmat(shmid, 0, 0);

  for ( i = 0; memoria[i] != '\0' && i < info.shm_segsz; i++)
  {
    printf("%c", memoria[i]);
  }

  esperarEnter();
}

void liberarMemoriaCompartida() {  
  int shmid;
  struct shmid_ds info;
  char *memoria;

  printf("Introduzca el shmid de la memoria compartida: ");
  scanf("%d%*c", &shmid);

  // TODO: Obtener información del bloque de memoria compartida y validar
  if (shmctl(shmid, IPC_STAT, &info) < 0) {
    printf("No existe un bloque de memoria con el shmid especificado.\n");
    esperarEnter();
    return;
  } 

  // TODO: Remover segmento de memoria compartida
  if(shmctl(shmid, IPC_RMID, 0) >= 0) 
    printf("Se ha borrado correctamente el bloque de memoria compartida.\n");
  else 
    printf("No se pudo borrar el bloque de memoria compartida.\n");
  
  esperarEnter();
}