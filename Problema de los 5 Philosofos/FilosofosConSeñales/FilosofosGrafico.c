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
#include <signal.h>

// Configuración
#define N 5

#define MIN_PENSAR 2
#define MAX_PENSAR 15

#define MIN_COMER 3
#define MAX_COMER 15

// Constantes y macros
#define LEFT  ((N + i - 1) % N)
#define RIGHT ((N + i + 1) % N)
#define limpia() printf("\033[H\033[J")

#define RADIO 100

#define MUTEX                -1
#define MUTEX_NOTIFICACIONES -2

#define THINKING  0
#define HUNGRY    1
#define EATING    2

/*
 * Variables Globales
 */
GtkWidget **imagenes;
GtkWidget *window;
int *estado;
int semidFilosofos, semidMutex, semidMutexNotificaciones;
int msqid, pipeNotificaciones[2];
int visible;
//
//--------------------Prototipos de funiones--------------------------//
//
//--------------------Funciones de aplicacion-------------------------//
void actualizarVisualizacion(int nuevoEstado[]);
static void activate(GtkApplication* app, gpointer user_data);
void inicializarAplicacion(int argc, char **argv);
//-------------------------Filosofos----------------------------------//
void filosofo(int i);
void tomarTenedores(int i);
void dejarTenedores(int i);
void prueba(int i);
void pensar();
void comer();
//--------------------Funciones de Semaforos--------------------------//

void up(int semaforo);
void down(int semaforo);
//-----------------------Comunicacion entre tuberias------------------//
void notificarCambio();
void recibirCambio(int estado[N]);
void *escucharNotificaciones(void *_);
int aplicarCambios(gpointer datos);
//<---------------------------------Señales-------------------------------------->//
void signalFuncion(int _);



//-------------------------------Main---------------------------------//
/**
 * Main
 */
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

	/*
	 * Señal SIGINT corresponde a ctrl c
	 */
	(void) signal(SIGINT, signalFuncion);
	visible = 1;

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
			close(pipeNotificaciones[0]);
			filosofo(i);
		}
	}
	//Crear Hilos
	if (!hijo) {
		pthread_t hiloNotificaciones; 
		pthread_create(&hiloNotificaciones, NULL, escucharNotificaciones, NULL);
		inicializarAplicacion(argc, argv);
	}
}
//--------------------Funciones de aplicacion-------------------------//
/*
 * Actualiza La imagen 
 */
void actualizarVisualizacion(int nuevoEstado[]) {
	int i, j, idx;
	for (i = 0; i < N; i++)
		for (j = 0; j < 3; j++) {
			idx = i * 3 + j;
			if (nuevoEstado[i] == j)
				gtk_widget_set_opacity(imagenes[idx], 1);
			else 
				gtk_widget_set_opacity(imagenes[idx], 0);
		}
}
/*
 * Eschucador de aplicacion
 */
static void activate(GtkApplication* app, gpointer user_data) {
	int i, j;
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "estilos.css", NULL);
	gtk_style_context_add_provider_for_screen(
		gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(cssProvider),
		GTK_STYLE_PROVIDER_PRIORITY_USER
	);
	//
	//Crea Ventana principal
	//
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Filosofos");
	gtk_window_set_default_size(GTK_WINDOW(window), 350, 300);
	//
	//Contenedor
	//
	GtkWidget *container = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), container);

	//
	//Calculo del tamaño de imagenes
	//
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
}
/*
 * Constructor de apicacion
 */
void inicializarAplicacion(int argc, char **argv) {
	GtkApplication *app;
	app = gtk_application_new(
		"Filosofos.Fatake", 
		G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
}
//-------------------------Filosofos----------------------------------//
//
//Filosofos
//
void filosofo(int i) {
	while (1) {
		pensar();
		tomarTenedores(i);
		comer();
		dejarTenedores(i);
	}
}
//
// Tomar tenedores
//
void tomarTenedores(int i) {
	down(MUTEX);
		estado[i] = HUNGRY;
		notificarCambio();
		prueba(i);
	up(MUTEX);
	down(i);
}
//
// Deja tenedores
//
void dejarTenedores(int i) {
	down(MUTEX);
		estado[i] = THINKING;
		notificarCambio();
		prueba(LEFT);
		prueba(RIGHT);
	up(MUTEX);
}
/*
 * Funcion que checa los filosofos de alado
 */
void prueba(int i) {
	if ((estado[i] == HUNGRY) &&
		(estado[LEFT] != EATING) &&
		(estado[RIGHT] != EATING)) {
		estado[i] = EATING;
		notificarCambio();
		up(i);
	}
}
//
// Funcion Pensar
//
void pensar() {
	int espera = MIN_PENSAR + rand() % (MAX_PENSAR - MIN_PENSAR + 1);
	time_t fin = time(NULL) + espera;
	while (time(NULL) < fin);
}
//
//Funcion comer
//
void comer() {
	int espera = MIN_COMER + rand() % (MAX_COMER - MIN_COMER + 1);
	time_t fin = time(NULL) + espera;

	while (time(NULL) < fin);
}
//--------------------Funciones de Semaforos-------------------------//
/*
 * Funcion Up recibe el un semid y 
 * incrementa en 1 su valor
 */
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
/*
 * Funcion down recibe un semid
 * y decrementa en 1 su valor o lo bloquea
 */
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

//-----------------------Comunicacion entre tuberias------------------//
void notificarCambio() {
	if (write(pipeNotificaciones[1], estado, sizeof(int) * N) < 0) {
		printf("Error: No se envio la notificacion u.u\n");
		exit(-1);
	}
}
void recibirCambio(int estado[N]) {
	if (read(pipeNotificaciones[0], estado, sizeof(int) * N) < 0) {
		printf("ERROR: No se pudo recibir una notificacion u.u \n");
		exit(-1);
	}
}
//
// Escucha de cambios en la tuberia
//
void *escucharNotificaciones(void *_) {
	int nuevoEstado[N];
	while (1) {
		down(MUTEX_NOTIFICACIONES);
		recibirCambio(nuevoEstado);
		limpia();
		printf("\n<---Filosfoso--->\n\n");
		int i;
		for (i = 0; i < N; i++){
			if(nuevoEstado[i] == 0)
				printf("P ");
			else if(nuevoEstado[i] == 1)
				printf("H ");
			else if(nuevoEstado[i] == 2)
				printf("C ");
		}
		printf("\n");
		gdk_threads_add_idle_full(G_PRIORITY_HIGH_IDLE, aplicarCambios, nuevoEstado, NULL);
	}
}
//
// Envia datos al actualizador de la pantalla
//
int aplicarCambios(gpointer datos) {
	int nuevoEstado[N];
	memcpy(nuevoEstado, datos, sizeof(int) * N);
	up(MUTEX_NOTIFICACIONES);
	actualizarVisualizacion(nuevoEstado);
	return 0;
}
//<---------------------------------Señales-------------------------------------->//
void signalFuncion(int _){
	if (visible == 1) {
		printf("\n\n\nCambiando a Modo Grafico\n\n\n");
    	gtk_widget_show_all(window);
		visible = 1;
  	} else {
		printf("\n\n\nCambiando a Terminal\n\n\n");
    	gtk_widget_hide(window);
		visible = 1;
  	}
}