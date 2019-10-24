#include <GL/glut.h>
/*
 * Compilacion
 * gcc FilosofosGrafico.c -lGL -lGLU -lglut -o Fil
 */

/*
 * Pantalla
 */
void pantalla() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

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
