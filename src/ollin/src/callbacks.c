/*
* $Id$
*
* callbacks.c for Linux Version 0.8.5
* Copyright (C) 2002 by Free Software Foundation, Inc.
* Author: Victor H De la Luz <itztli@gmail.com>
*
*                         COPYING
*
* This  program  is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License  as
* published by the Free Software Foundation; either version 2, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty  of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE.  See  the
* GNU General Public License for more details.
*
* You  should  have  received  a  copy  of the GNU General Public
* License along with this program; if  not,  write  to  the  Free
* Software Foundation, Inc., 675  Mass  Ave, Cambridge, MA 02139,
* USA.
*
* ########################## OVERVIEW ###########################
*
* This file contains the  callbacks  main  functions  to  tochtli
* sub program.
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005 
* ###############################################################
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include "ImageEngine.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

extern int *Memoria;

GdkPixmap *pixmap = NULL; //area de Dibujo en Segundo Plano!

gboolean
on_drawingareaPrincipal_configure_event(GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
   if (pixmap){
    gdk_pixmap_unref(pixmap);
  }
  pixmap = gdk_pixmap_new(widget->window,
			 widget->allocation.width,
			 widget->allocation.height,
			 -1);
  return TRUE;

}


gboolean
on_drawingareaPrincipal_expose_event   (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  pixmap,
		  event->area.x,event->area.y,
		  event->area.x,event->area.y,
		  event->area.width,event->area.height);
  return FALSE;  
}

gboolean
on_scrolledwindow_button_press_event   (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

  return FALSE;
}


gboolean
on_windowPrincipal_delete_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  //  gtk_main_quit();
  return TRUE;
}


gint revisarCambios(gpointer data){
  time_t now;
  struct tm *now_tm;
  char archivo[100];
  FILE *fileXML = NULL;
  FILE *file = NULL;

  if(Memoria[27] == 100){ //TERMINAMOS
    while (gtk_events_pending()) {
      gtk_main_iteration_do(0);
    }
    gtk_main_quit();
  }

  if(Memoria[0]){
      Memoria[0] = 0; //Lo estamos atendiendo
    if (Memoria[1] & Memoria[2]){ //Ok, vamos a capturar con un nuevo projecto!!
      Memoria[2] = 0;
      //sincronizando el sistema con el servidor CRONOS.
      Memoria[24]= 5;
      printf("\nSincronizando reloj con cronos...\n");
      system("sudo ntpdate cronos.cenam.mx"); //sincronizando
      Memoria[24]= 30;
      printf("\nActualizando Hardware...\n");
      system("sudo /etc/init.d/hwclock.sh restart"); //copiando a HW
      Memoria[24]= 40;
      //Escuchando la hora    
      time(&now);  //Pido la hora
      now_tm = gmtime(&now); //la hora la transforma a UTC
      //creando el directorio de trabajo a partir de la fecha
      sprintf(archivo,"/home/database/%i/%i/%i/README.xml",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      Memoria[24]= 50;
      fileXML = fopen(archivo,"a"); //destino

      sprintf(archivo,"/home/database/%i/%i/%i/README",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      file = fopen(archivo,"a"); //destino
      Memoria[24]= 60;
      fprintf(fileXML,"\n <modificacion>\n  <tipo>Inicia Captura</tipo>\n   <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n   <dato/>\n </modificacion>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);
      fprintf(file,"\n--->Inicia Captura : %i:%i.%i\n",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);
      Memoria[24]= 70;
      fprintf(fileXML,"\n <modificacion>\n  <tipo>Amplificacion</tipo>\n   <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n   <dato>%i</dato>\n </modificacion>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[4]);
      fprintf(file,"*Amplificacion : %i:%i.%i  Valor : %i\n",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[4]);

      Memoria[24]= 80;
      fprintf(file,"*Frecuencia : %i:%i.%i  Valor : %i\n",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[5]);
      fprintf(fileXML,"\n <modificacion>\n  <tipo>Frecuencia</tipo>\n   <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n   <dato>%i</dato>\n </modificacion>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[5]);
      Memoria[24]= 90;
      fclose(fileXML);
      fclose(file);
      sprintf(archivo,"/home/database/%i/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);    //asi quedo la ruta de hoy!
      Memoria[24]= 100;
      grafica_main(data,archivo,1); //continuamos
    }else if (Memoria[2]){ //Ok, vamos a comenzar un nuevo projecto!!
      //sincronizando el sistema con el servidor CRONOS.
      Memoria[24]= 5;
      //      printf("\nSincronizando reloj con cronos...\n");
      //system("sudo ntpdate cronos.cenam.mx"); //sincronizando
      Memoria[24]= 30;
      //printf("\nActualizando Hardware...\n");
      //system("sudo /etc/init.d/hwclock.sh restart"); //copiando a HW
      Memoria[24]= 40;
      //Escuchando la hora    
      time(&now);  //Pido la hora
      now_tm = gmtime(&now); //la hora la transforma a UTC
      //creando el directorio de trabajo a partir de la fecha
      printf("\nCreando Directorios...\n");
      sprintf(archivo,"mkdir /home/database/");    
      system(archivo); //creamos el directorio
      Memoria[24]= 50;
      sprintf(archivo,"mkdir /home/database/%i",now_tm->tm_year+1900);    
      system(archivo); //creamos el directorio
      Memoria[24]= 60;
      sprintf(archivo,"mkdir /home/database/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1);
      system(archivo); //creamos el directorio
      Memoria[24]= 70;
      sprintf(archivo,"mkdir /home/database/%i/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);    
      system(archivo); //creamos el directorio
      Memoria[24]= 80;

      sprintf(archivo,"/home/database/%i/%i/%i/README.xml",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      fileXML = fopen(archivo,"wb"); //destino

      sprintf(archivo,"/home/database/%i/%i/%i/README",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      file = fopen(archivo,"wb"); //destino

      fprintf(fileXML,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone='no'?>");
      fprintf(fileXML,"\n<!DOCTYPE xquetzal SYSTEM \"http://cintli.igeofcu.unam.mx/ris/xquetzal.dtd\">");
      fprintf(fileXML,"\n<?xml-stylesheet href=\"http://cintli.igeofcu.unam.mx/ris/xquetzal.xsl\" type=\"text/xsl\"?>");
      fprintf(fileXML,"\n<!-- Archivo Generado por Xquetzal, no modificar. -->\n");
      fprintf(fileXML,"<!-- Hora en forma UTC/GTM. -->\n");
      fprintf(file,"<!-- Archivo Generado por Xquetzal, no modificar. -->\n");
      fprintf(file,"<!-- Hora en forma UTC/GTM. -->\n");
      fprintf(fileXML,"<xquetzal>");
      fprintf(fileXML,"\n <creacion>\n  <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n </creacion>\n<projecto>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);
      fprintf(file,">Creacion del Proyecto : %i/%i/%i %i:%i.%i\n",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday,now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);
      Memoria[24]= 90;
      fclose(fileXML);
      fclose(file);
      sprintf(archivo,"/home/database/%i/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);    //asi quedo la ruta de hoy!
      Memoria[24]= 100;
    }else if (Memoria[1]){ //Ok, vamos a continuar
      //Escuchando la hora    
      time(&now);  //Pido la hora
      now_tm = gmtime(&now); //la hora la transforma a UTC
      //creando el directorio de trabajo a partir de la fecha
      sprintf(archivo,"/home/database/%i/%i/%i/README.xml",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      Memoria[24]= 20;
      fileXML = fopen(archivo,"a"); //destino

      sprintf(archivo,"/home/database/%i/%i/%i/README",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      file = fopen(archivo,"a"); //destino

      fprintf(fileXML,"\n <modificacion>\n  <tipo>Inicia Captura</tipo>\n   <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n   <dato/>\n </modificacion>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);
      fprintf(file,"\n--->Inicia Captura :  %i:%i.%i\n",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);
      fclose(fileXML);
      fclose(file);
      Memoria[24]= 50;
      sprintf(archivo,"/home/database/%i/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);    //asi quedo la ruta de hoy!
      Memoria[24]= 100;
      grafica_main(data,archivo,0); //continuamos
    }else if(Memoria[27] == 1){ //CAMBIO DE AMPLIFICACION!!!
      Memoria[1] = 1; //vamos a capturar de nuevo!
      g_print("\n Cambio de amplificacion\n");
      //Escuchando la hora    
      time(&now);  //Pido la hora
      now_tm = gmtime(&now); //la hora la transforma a UTC
      //creando el directorio de trabajo a partir de la fecha
      sprintf(archivo,"/home/database/%i/%i/%i/README.xml",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      fileXML = fopen(archivo,"a"); //destino
      sprintf(archivo,"/home/database/%i/%i/%i/README",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      file = fopen(archivo,"a"); //destino


      fprintf(fileXML,"\n <modificacion>\n  <tipo>Amplificacion</tipo>\n   <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n   <dato>%i</dato>\n </modificacion>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[4]);
      fprintf(file,"Cambio de Amplificacion : %i:%i.%i Valor: %i\n",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[4]);

      fclose(fileXML);
      fclose(file);
      sprintf(archivo,"/home/database/%i/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);    //asi quedo la ruta de hoy!
      grafica_main(data,archivo,2); //continuamos con un cambio de amplificacion
    }else if(Memoria[27] == 2){//CAMBIO DE FRECUENCIA
      Memoria[1] = 1; //vamos a capturar de nuevo!
      g_print("\n Cambio de Frecuencia");
      //Escuchando la hora    
      time(&now);  //Pido la hora
      now_tm = gmtime(&now); //la hora la transforma a UTC
      //creando el directorio de trabajo a partir de la fecha
      sprintf(archivo,"/home/database/%i/%i/%i/README.xml",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      fileXML = fopen(archivo,"a"); //destino
      sprintf(archivo,"/home/database/%i/%i/%i/README",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);
      file = fopen(archivo,"a"); //destino
      fprintf(fileXML,"\n <modificacion>\n  <tipo>Frecuencia</tipo>\n   <time><hora>%i</hora><minuto>%i</minuto><segundo>%i</segundo></time>\n   <dato>%i</dato>\n </modificacion>",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[5]);
      fprintf(file,"Cambio de Frecuencia : %i:%i.%i\n   Valor: %i\n",now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,Memoria[5]);
      fclose(fileXML);
      fclose(file);
      sprintf(archivo,"/home/database/%i/%i/%i",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday);    //asi quedo la ruta de hoy!
      grafica_main(data,archivo,3); //continuamos con un cambio de frecuencia
    }
  }
  return(1);
}











