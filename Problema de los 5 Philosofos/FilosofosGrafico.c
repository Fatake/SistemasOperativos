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

/*
 * Main
 */
int main(int argc, char** argv) {
   glutInit(&argc, argv);                 // Initialize GLUT
   glutCreateWindow("Filosofos Grafico"); // Create a window with the given title
   glutInitWindowSize(1000, 1000);   // Set the window's initial width & height
   glutInitWindowPosition(400, 500); // Position the window's initial top-left corner
   glutDisplayFunc(pantalla); // Register display callback handler for window re-paint
   glutMainLoop();           // Enter the infinitely event-processing loop
   return 0;
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


   // Draw a Red 1x1 Square centered at origin
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(-0.5f, -0.5f);    // x, y
      glVertex2f( 0.5f, -0.5f);
      glVertex2f( 0.5f,  0.5f);
      glVertex2f(-0.5f,  0.5f);
   glEnd();

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
