#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define TAMANO_MENSAJE 100

void crearColaMensajes();
void enviarMensaje();
void recibirMensaje();
void eliminarColaMensajes();

void operacionesMensajes() {
  do {
    limpiarPantalla();
    printf("=== Operaciones de mensajes ===\n");
    printf("1. Crear cola de mensajes\n");
    printf("2. Enviar mensaje\n");
    printf("3. Recibir mensaje\n");
    printf("4. Eliminar cola de mensajes\n");
    printf("5. Regresar\n");

    switch (obtenerOpcion()) {
      case 1:
        crearColaMensajes();
        break;

      case 2:
        enviarMensaje();
        break;

      case 3:
        recibirMensaje();
        break;

      case 4:
        eliminarColaMensajes();
        break;

      case 5:
        return;
      default:
        printf("\nNo se reconoció la opción.\n");
        esperarEnter();
    }

  } while (1);
}

struct mensaje {
  long mtype;
  char mtext[TAMANO_MENSAJE];
};

void crearColaMensajes() {
  int permisos, msqid;

  printf("Ingrese los permisos de la cola de mensajes (en octal): ");
  scanf("%o%*c", &permisos);

  msqid = msgget(IPC_PRIVATE, IPC_CREAT | permisos);
  if (msqid >= 0) 
    printf("Se creó exitosamente la cola de mensajes con msqid %d.\n", msqid);
  else 
    printf("Ocurrió un error al crear la cola de mensajes.\n");
  
  esperarEnter();
}

void enviarMensaje() {
  int tipo, msqid;
  struct mensaje msj;
  struct msqid_ds info;

  printf("Ingrese el msqid de la cola de mensajes: ");
  scanf("%d%*c", &msqid);

  if (msgctl(msqid, IPC_STAT, &info) < 0) {
    printf("La cola de mensajes especificada no existe.\n");
    esperarEnter();
    return;
  }

  printf("Ingrese el tipo de mensaje que desea enviar: ");
  scanf("%d%*c", &tipo);
  msj.mtype = tipo;

  printf("Ingrese el mensaje que desea enviar:\n");
  fgets(msj.mtext, TAMANO_MENSAJE, stdin);

  if(msgsnd(msqid, &msj, sizeof(struct mensaje), 0) >= 0) 
    printf("El mensaje se envió correctamente.\n");
  else 
    printf("No se pudo enviar el mensaje.\n");

  esperarEnter();
}

void recibirMensaje() {
  int tipo, msqid;
  struct mensaje msj;
  struct msqid_ds info;

  printf("Ingrese el msqid de la cola de mensajes: ");
  scanf("%d%*c", &msqid);

  if (msgctl(msqid, IPC_STAT, &info) < 0) {
    printf("La cola de mensajes especificada no existe.\n");
    esperarEnter();
    return;
  }

  printf("Ingrese el tipo de mensaje que desea recibir: ");
  scanf("%d%*c", &tipo);

  if (msgrcv(msqid, &msj, sizeof(struct mensaje), tipo, 0) < 0) {
    printf("No se pudo recibir un mensaje con el tipo especificado.\n");
    esperarEnter();
    return;
  }

  printf("El mensaje recibido es:\n%s", msj.mtext);
  esperarEnter();
}

void eliminarColaMensajes() {
  int msqid;
  struct msqid_ds info;

  printf("Ingrese el msqid de la cola de mensajes a borrar: ");
  scanf("%d%*c", &msqid);

  if (msgctl(msqid, IPC_STAT, &info) < 0) {
    printf("La cola de mensajes especificada no existe.\n");
    esperarEnter();
    return;
  }
  
  if(msgctl(msqid, IPC_RMID, 0) >= 0) 
    printf("La cola de mensajes se borró correctamente.\n");
  else 
    printf("Ocurrió un error al borrar la cola de mensajes.\n");

  esperarEnter();
}