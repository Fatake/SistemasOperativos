#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

// Configuración
#define N 5

#define MIN_PENSAR 3
#define MAX_PENSAR 5

#define MIN_COMER 2
#define MAX_COMER 4

#define LEFT  ((N + i - 1) % N)
#define RIGHT ((N + i + 1) % N)

#define MUTEX    -1

#define THINKING  0
#define HUNGRY    1
#define EATING    2

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};  

void filosofo(int);
void up(int semaforo), down(int semaforo);

int *estado;
int semidFilosofos, semidMutex;

int main (int argc, char **argv) {
  int i, hijo = 0;
  unsigned short iniciales[N];
  union semun operaciones;

  estado = shmat(shmget(IPC_PRIVATE, sizeof(int) * N, IPC_CREAT | 0666), 0, 0);
  for (i = 0; i < N; i++) {
    estado[i] = THINKING;
    iniciales[i] = 1;
  }

  semidFilosofos = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);
  semidMutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

  operaciones.array = iniciales;
  semctl(semidFilosofos, 0, SETVAL, operaciones);

  operaciones.val = 1;
  semctl(semidMutex, 0, SETVAL, operaciones);

  for (i = 0; i < N; i++) {
    if (fork() == 0) {
      hijo = 1;
      filosofo(i);
    }
  }

  if (!hijo) {
    while (1) {
      down(MUTEX);
      for (i = 0; i < N; i++) {
        printf("%d ", estado[i]);
      }
      printf("\n");
      up(MUTEX);
      
      sleep(1);
    }
  }
}

void pensar(), comer();
void dejarTenedores(int), tomarTenedores(int);

void filosofo(int i) {
  while (1) {
    pensar();
    tomarTenedores(i);
    comer();
    dejarTenedores(i);
  }
}

void prueba(int);

void dejarTenedores(int i) {
  down(MUTEX);
  estado[i] = THINKING;
  prueba(LEFT);
  prueba(RIGHT);
  up(MUTEX);
}

void tomarTenedores(int i) {
  down(MUTEX);
  estado[i] = HUNGRY;
  prueba(i);
  up(MUTEX);
  down(i);
}

void prueba(int i) {
  if ((estado[i] == HUNGRY) &&
      (estado[LEFT] != EATING) &&
      (estado[RIGHT] != EATING)) {
    estado[i] = EATING;
    up(i);
  }
}

void pensar() {
  int espera = MIN_PENSAR + rand() % (MAX_PENSAR - MIN_PENSAR + 1);
  time_t fin = time(NULL) + espera;
  
  while (time(NULL) < fin);
}

void comer() {
  int espera = MIN_COMER + rand() % (MAX_COMER - MIN_COMER + 1);
  time_t fin = time(NULL) + espera;
  
  while (time(NULL) < fin);
} 

void up(int semaforo) {
  struct sembuf op = { 
    .sem_num = 0, 
    .sem_op = 1, 
    .sem_flg = 0 
  };

  if (semaforo == MUTEX) {
    semop(semidMutex, &op, 1);
  } else {
    op.sem_num = semaforo;
    semop(semidFilosofos, &op, 1);
  }
}

void down(int semaforo) {
  struct sembuf op = { 
    .sem_num = 0, 
    .sem_op = -1, 
    .sem_flg = 0 
  };

  if (semaforo == MUTEX) {
    semop(semidMutex, &op, 1);
  } else {
    op.sem_num = semaforo;
    semop(semidFilosofos, &op, 1);
  }
}
