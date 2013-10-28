#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

key_t Clave;
int Id_Memoria;
int *Memoria = NULL;
cairo_surface_t *superficie = NULL;

/* Esta función inicializa la superficie y la deja limpia*/
void limpia_superficie(){
  /* Declaramos algo del tipo cairo */
  cairo_t *cr;
  /* La biblioteca cairo se encarga de generar todos las variables en donde se va a dibujar*/
  cr = cairo_create(superficie);

  /* Establecemos el color inicial de la superficie */
  cairo_set_source_rgb(cr, 1, 1, 1);

  /* Pintamos la superficie */
  cairo_paint(cr);

  /* Destruimos la variable ya que no es necesaria en este momento */
  cairo_destroy(cr);
}

/*
 * Esta función tiene como objetivo el terminar la ejecución de la aplicación.
 */
void on_windowPrincipal_destroy(GtkWidget *object, gpointer user_data){
  gtk_main_quit();
}

/*
 * Aquí creamos una superficie de dibujo en la cual vamos a almacenar todo.
 */
gboolean on_drawingareaPrincipal_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data){
  /* Verificamos que la superficie no esté en uso */
  if(superficie)
    cairo_surface_destroy(superficie);

  /* Creamos una nueva instancia de la superficie */
  superficie = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
					   CAIRO_CONTENT_COLOR,
					   gtk_widget_get_allocated_width (widget),
					   gtk_widget_get_allocated_height (widget));

  /* Iniciamos con una superficie limpia */
  limpia_superficie();

  
  /* En este punto hemos manejado satisfactoriamente el evento. */
  return TRUE;
}

/* Esta función se encarga de dibujar en la superficie*/
gboolean on_windowPrincipal_draw (GtkWidget *widget, cairo_t *cr, gpointer user_data){
  /* Establecemos la variable que será la superficie de dibujo */
  cairo_set_source_surface (cr, superficie, 0, 0);
  
  /* Dibujamos la superficie */
  cairo_paint(cr);
  
  return FALSE;
}

/* Función que maneja el evento de borrado*/
gboolean on_windowPrincipal_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data){
  return TRUE;
}

int main (int argc, char *argv[])
{

  GtkBuilder *builder;
  GtkWidget *window;
  //GtkWidget *windowPrincipal;

  Clave = ftok("/bin/ls",33);
  if (Clave == -1){
    printf("No consigo clave para memoria compartida\n");
    exit(0);
  }
  Id_Memoria = shmget(Clave,sizeof(int)*36,0777);
  if (Id_Memoria == -1){
    printf("No consigo Id para memoria compartida\n");
    exit(0);
  }
  Memoria = (int*)shmat(Id_Memoria,(char*)0,0);
  if (Memoria == NULL){
    printf("No consigo memoria compartida\n");
    exit(0);
  }  


  /* gtk_set_locale (); */
  /* gtk_init (&argc, &argv); */

  /* add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps"); */
  /* add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps"); */

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  /* windowPrincipal = create_windowPrincipal (); */
  /* gtk_widget_show (windowPrincipal); */

  /* gtk_main (); */

  gtk_init(&argc, &argv);
  
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "ollin.xml", NULL);
  gtk_builder_connect_signals(builder, NULL);
  window = GTK_WIDGET (gtk_builder_get_object (builder, "windowPrincipal"));
  g_object_unref(G_OBJECT(builder));
  
  gtk_widget_show(window);
  gtk_main();

  if(Id_Memoria != -1){
    shmdt((char*)Memoria);
  } 
  exit(0);
  return 1;
}

