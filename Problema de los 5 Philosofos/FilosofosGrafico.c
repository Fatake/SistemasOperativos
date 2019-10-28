/*
 * Compilacion
 * gcc FilosofosGrafico.c -lGL -lGLU -lglut -o Fil
 */
#include "ipcfunctions.h"
#include <stdio.h>
#include <GL/glut.h>

//Macros y Definiciones
#define N 5
#define left (N+i-1)%N
#define right (N+i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define True 1
#define False 0
#define TAM 10

//Tipos
typedef int semaforo;

int estado[N];
semaforo mutex;
semaforo sem[N];
int i;

void filosofo(int numeroFilosofo);
void takeForks(int i);
void putForks(int i);
void comprueba(int i);

/*
 * Pantalla
 */
void pantalla();
void reshape(int width,int height);

/*
 * Main
 */
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(600, 500); // Position the window's initial top-left corner
	glutCreateWindow("Filosofos Grafico"); // Create a window with the given title

	glutDisplayFunc(pantalla); // Register display callback handler for window re-paint
	glutReshapeFunc(reshape);
	glutMainLoop();           // Enter the infinitely event-processing loop

	return 0;
}

void reshape(int width,int height){
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100,100,-100,100);
	glMatrixMode(GL_MODELVIEW);
}
//
//Funciones
//
/*
 * Constructor de la pantalla Grafica
 */
void pantalla(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	//inicializar el estado en pensando
	for(i = 0; i < N ; i++)
		estado[i] = 0;
	//Semaforo de mutex
	mutex = creaSemaforo(0,1);
	//Semaforos para cada Filosofo
	for(i = 0; i < N ; i++)
		sem[i] = creaSemaforo(0,0);

	printf("\nSemaforo mutex : %d\n",mutex);
	for ( i = 0; i < N; i++)
		printf("Semaforo numero %d : %d\n",(i+1),sem[i]);


	glPushMatrix();//Rescata el escenario
	glTranslated(50,-50,0);
		glBegin(GL_QUADS);//Crea un Cuadrado
			glColor3f(1.0f, 0.0f, 0.0f);//Color Rojo
			glVertex2f(-TAM, -TAM);    // x, y
			glVertex2f( TAM, -TAM);
			glVertex2f( TAM,  TAM);
			glVertex2f(-TAM,  TAM);
		glEnd();
	glPopMatrix();


	glPushMatrix();

	glTranslated(-50,-50,0);

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(-TAM, -TAM);    // x, y
	glVertex2f( TAM, -TAM);
	glVertex2f( TAM,  TAM);
	glVertex2f(-TAM,  TAM);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glTranslated(-50,50,0);

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(-TAM, -TAM);    // x, y
	glVertex2f( TAM, -TAM);
	glVertex2f( TAM,  TAM);
	glVertex2f(-TAM,  TAM);
	glEnd();
	glPopMatrix();


	glPushMatrix();

	glTranslated(50,50,0);

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(-TAM, -TAM);    // x, y
	glVertex2f( TAM, -TAM);
	glVertex2f( TAM,  TAM);
	glVertex2f(-TAM,  TAM);
	glEnd();

	glPopMatrix();
	glPushMatrix();

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(-TAM, -TAM);    // x, y
	glVertex2f( TAM, -TAM);
	glVertex2f( TAM,  TAM);
	glVertex2f(-TAM,  TAM);
	glEnd();

	glPopMatrix();

	glFlush();  // Render now
}
/*
 * Funcion Filososofo
 */
void filosofo(int numeroFilosofo){
	while (True){
		printf("\nPensando----\n");
		takeForks(numeroFilosofo);
		printf("\nComiendo----\n");
		putForks(numeroFilosofo);
	}

}
/*
 * Funcion Tomar los Tenedores
 */
void takeForks(int i){
	semDecre(mutex);//Down
	estado[i] = HUNGRY;
	comprueba(i);
	semIncre(mutex);//Up
	semDecre(sem[i]);//Down
}
/*
 * Funcion Dejar los tenedores
 */
void putForks(int i){
	semDecre(mutex);//Down
	estado[i] = THINKING;
	comprueba(left);
	comprueba(right);
	semIncre(mutex);//Up
}
/*
 * Funcion Comprueba
 */
void comprueba(int i){
	if(estado[i] == HUNGRY &&
	estado[left] != EATING &&
	estado[right] != EATING){
		estado[i] = EATING;
		semIncre(sem[i]);
	}
}
