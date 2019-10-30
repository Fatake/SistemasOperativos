#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

// Configuración
#define N 5

#define MIN_PENSAR 2
#define MAX_PENSAR 10

#define MIN_COMER 3
#define MAX_COMER 15

/* Configuración interfaz */
#define RADIO 180

// Constantes y macros
#define LEFT  ((N + i - 1) % N)
#define RIGHT ((N + i + 1) % N)

#define MUTEX                -1
#define MUTEX_NOTIFICACIONES -2

#define THINKING  0
#define HUNGRY    1
#define EATING    2

// Lógica de visualización

GtkWidget **imagenes;
void actualizarVisualizacion(int nuevoEstado[]) {
	int i, j, idx;
	for (i = 0; i < N; i++) {
		for (j = 0; j < 3; j++) {
			idx = i * 3 + j;

			if (nuevoEstado[i] == j)
				gtk_widget_set_opacity(imagenes[idx], 1);
			else 
				gtk_widget_set_opacity(imagenes[idx], 0);
		}
	}
}

static void activate(GtkApplication* app, gpointer user_data) {
	int i, j;

	// Cargar estilos
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "Prueba1.css", NULL);
	gtk_style_context_add_provider_for_screen(
	gdk_screen_get_default(),
	GTK_STYLE_PROVIDER(cssProvider),
	GTK_STYLE_PROVIDER_PRIORITY_USER);

	// Crear ventana principal
	GtkWidget *window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Filósofos");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

	GtkWidget *container = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), container);

	double angulo = 2.0 * G_PI / (double) N;
	float escalas[] = { 0.18, 0.25, 0.15 };
	char *nombreImagenes[] = { "pensando.gif", "hambriento.gif", "comiendo.gif" };

	char idImagen[15];
	char estilo[200];

	imagenes = malloc(sizeof(GtkWidget *) * N * 3);
	for (i = 0; i < N; i++) {
		for (j = 0; j < 3; j++) {
			int idx = i * 3 + j;
			int x = (int) (cos(angulo * i - G_PI_2) * RADIO);
			int y = (int) (sin(angulo * i - G_PI_2) * RADIO);

			sprintf(idImagen, "imagen-%d-%d", i, j);
			sprintf(
				estilo,
				"#imagen-%d-%d { -gtk-icon-transform: translate(%dpx, %dpx) scale(%.2f); }",
			i, j, x, y, escalas[j]);

			imagenes[idx] = gtk_image_new_from_file(nombreImagenes[j]);

			gtk_widget_set_name(imagenes[idx], idImagen);

			GtkCssProvider *proveedor = gtk_css_provider_new();
			gtk_css_provider_load_from_data(proveedor, estilo, strlen(estilo), NULL);
			gtk_style_context_add_provider_for_screen(
			gdk_screen_get_default(),
			GTK_STYLE_PROVIDER(proveedor),
			GTK_STYLE_PROVIDER_PRIORITY_USER);

			gtk_fixed_put(GTK_FIXED(container), imagenes[idx], 0, 0);
		}
	}

	int estadoInicial[N];
	for (i = 0; i < N; i++)
		estadoInicial[i] = THINKING;

	actualizarVisualizacion(estadoInicial);

	gtk_widget_show_all(window);
}

void inicializarAplicacion(int argc, char **argv) {
	GtkApplication *app;

	app = gtk_application_new(
		"org.ggzor.collab.paquetito.filosofos", 
		G_APPLICATION_FLAGS_NONE);

	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
}

int *estado;
int semidFilosofos, semidMutex, semidMutexNotificaciones;
int msqid, pipeNotificaciones[2];

void filosofo(int);
void *escucharNotificaciones(void *);

int main (int argc, char **argv) {
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *_buf;
	};  

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
	semidMutexNotificaciones = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

	operaciones.array = iniciales;
	semctl(semidFilosofos, 0, SETVAL, operaciones);

	operaciones.val = 1;
	semctl(semidMutex, 0, SETVAL, operaciones);

	operaciones.val = 1;
	semctl(semidMutexNotificaciones, 0, SETVAL, operaciones);

	pipe(pipeNotificaciones);

	for (i = 0; i < N; i++) {
		if (fork() == 0) {
			hijo = 1;

			// Cerrar pipe en cada hijo para lectura
			close(pipeNotificaciones[0]);

			filosofo(i);
		}
	}

	if (!hijo) {
		// Cerrar el pipe en el padre para escritura

		pthread_t hiloNotificaciones; 
		pthread_create(&hiloNotificaciones, NULL, escucharNotificaciones, NULL);

		inicializarAplicacion(argc, argv);
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
void notificarCambio();
void up(int semaforo), down(int semaforo);

void tomarTenedores(int i) {
	down(MUTEX);

	estado[i] = HUNGRY;
	notificarCambio();

	prueba(i);
	up(MUTEX);
	down(i);
}

void dejarTenedores(int i) {
	down(MUTEX);

	estado[i] = THINKING;
	notificarCambio();

	prueba(LEFT);
	prueba(RIGHT);
	up(MUTEX);
}

void prueba(int i) {
	if ((estado[i] == HUNGRY) &&
		(estado[LEFT] != EATING) &&
		(estado[RIGHT] != EATING)) {

		estado[i] = EATING;
		notificarCambio();

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

	if (semaforo == MUTEX)
		semop(semidMutex, &op, 1);
	else if (semaforo == MUTEX_NOTIFICACIONES)
		semop(semidMutexNotificaciones, &op, 1);
	else {
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

	if (semaforo == MUTEX)
		semop(semidMutex, &op, 1);

	else if (semaforo == MUTEX_NOTIFICACIONES)
		semop(semidMutexNotificaciones, &op, 1);

	else {
		op.sem_num = semaforo;
		semop(semidFilosofos, &op, 1);
	}
}


// Notificación de cambios
void notificarCambio() {
	if (write(pipeNotificaciones[1], estado, sizeof(int) * N) < 0) {
		printf("ERROR: No se pudo enviar una notificación.");
		exit(-1);
	}
}

int recibirCambio(int estado[N]) {
	if (read(pipeNotificaciones[0], estado, sizeof(int) * N) < 0) {
		printf("ERROR: No se pudo recibir una notificación.");
		exit(-1);
	}
}

// Aplicación de cambios
int aplicarCambios();
void *escucharNotificaciones(void *_) {
	int nuevoEstado[N];
	while (1) {
		down(MUTEX_NOTIFICACIONES);
		recibirCambio(nuevoEstado);

		printf("Recibiendo notificaciones: ");
		int i;
		for (i = 0; i < N; i++)
			printf("%d ", nuevoEstado[i]);

		printf("\n");

		gdk_threads_add_idle_full(G_PRIORITY_HIGH_IDLE, aplicarCambios, nuevoEstado, NULL);
	}
}

int aplicarCambios(gpointer datos) {
	int nuevoEstado[N];
	memcpy(nuevoEstado, datos, sizeof(int) * N);
	up(MUTEX_NOTIFICACIONES);

	actualizarVisualizacion(nuevoEstado);
	return 0;
}
