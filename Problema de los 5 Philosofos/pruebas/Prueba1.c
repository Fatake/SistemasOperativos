#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 5
#define RADIO 180

int fase = 0;
int estado[N] = { 0, 1, 1, 2, 0 };
GtkWidget **imagenes;

#define THINKING 0
#define HUNGRY   1
#define EATING   2

void actualizarEstado() {
  int i, j, idx;
  for (i = 0; i < N; i++) {
    for (j = 0; j < 3; j++) {
      idx = i * 3 + j;

      if (estado[i] == j)
        gtk_widget_set_opacity(imagenes[idx], 1);
      else 
        gtk_widget_set_opacity(imagenes[idx], 0);
    }
  }
}

void change(GtkWidget *button, gpointer data) {
  int fase0[] = { 0, 1, 1, 2, 0 };
  int fase1[] = { 1, 2, 0, 2, 1 };
  int *nuevaFase;
  int i;

  fase ^= 1;

  nuevaFase = fase == 1 ? fase1 : fase0;

  for (i = 0; i < N; i++)
    estado[i] = nuevaFase[i];

  actualizarEstado();
}

static void activate(GtkApplication* app, gpointer user_data) {
  GtkWidget *window, *container;
  GtkStyleContext *context;

  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "Prueba1.css", NULL);
  gtk_style_context_add_provider_for_screen(
    gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(cssProvider),
    GTK_STYLE_PROVIDER_PRIORITY_USER);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  container = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), container);

  GtkWidget *button = gtk_button_new_with_label("Change");
  g_signal_connect(GTK_WIDGET(button), "clicked", G_CALLBACK(change), NULL);

  GtkWidget *box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(box), button);
  gtk_fixed_put(GTK_FIXED(container), box, 250, 250);

  int i, j, idx, x, y;
  double angulo = 2.0 * G_PI / (double) N;
  float escalas[] = { 0.18, 0.25, 0.15 };
  char *nombreImagenes[] = { "pensando.gif", "hambriento.gif", "comiendo.gif" };
  char idImagen[15];
  char estilo[200];

  imagenes = malloc(sizeof(GtkWidget *) * N * 3);
  for (i = 0; i < N; i++) {
    for (j = 0; j < 3; j++) {
      idx = i * 3 + j;
      x = (int) (cos(angulo * i - G_PI_2) * RADIO);
      y = (int) (sin(angulo * i - G_PI_2) * RADIO);

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


  gtk_widget_show_all(window);
  actualizarEstado();
}

int actualizarInterfaz(gpointer *data) {
  change(NULL, NULL);
  return 0;
}

void *esperarMensajes(void *_) {
  while (1) {
    gdk_threads_add_idle(G_SOURCE_FUNC(actualizarInterfaz), NULL);
    sleep(2);
  }
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  pthread_t hiloMensajes;
  pthread_create(&hiloMensajes, NULL, esperarMensajes, NULL);

  app = gtk_application_new(
    "org.ggzor.collab.paquetito.filosofos", 
    G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}