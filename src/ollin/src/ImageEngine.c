/*
* $Id$
*
* ImageEngine.c for Linux Version 0.8.6
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
* Image Engine to xquetzal client program. 
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005
* 0.8.6 12/Abril/2005 BUG Posible mal guardado.
* ###############################################################
*/

#include <time.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "ImageEngine.h"
#include "i_addr.h"
#include "Guardar.h"
#include "Buffer.h"

//Memoria compartida
extern int *Memoria;

gint *datosPantalla0; //DATOS
gint *datosPantalla1; //DATOS
gint *datosPantalla2; //DATOS
gint *datosPantalla3; //DATOS

extern GdkPixmap *pixmap; //area de Dibujo en Segundo Plano!
gint separacion = 1;
/*Pruebas*/
//time_t horaInicio;
//time_t horaFin;
/*Pruebas*/
/*colores*/
static GdkGC *rojo = NULL;
static GdkGC *verde = NULL;
static GdkGC *azul = NULL;
static GdkGC *negro = NULL;
static GdkGC *gris = NULL;
static GdkGC *transparente = NULL;

static gint CERO0 = -1;
static gint CERO1;
static gint CERO2;
static gint CERO3;

static gint A,B;


GdkGC *GetPen(GdkColor *c){
  GdkGC *gc;
  gc = gdk_gc_new(pixmap);
  gdk_gc_set_foreground(gc,c);
  return(gc);

}

GdkColor *NewColor(long red, long green, long blue){
  GdkColor *c = (GdkColor *)g_malloc(sizeof(GdkColor));
  c->red =red;
  c->green =green;
  c->blue =blue;
  gdk_color_alloc(gdk_colormap_get_system(),c);
  return(c);
}






int hora2int(int hora, int minuto, int segundo){
  int horaint = 0;
  int hora12 = hora;
  if (hora > 12) { //es p.m.
    horaint = 0x010000; 
    hora12 = hora - 12;
  }
  return (horaint | (hora12 << 12) | (minuto << 6) | segundo);
}

void escala (gpointer data){
  /*  int k;
  for (k=0)
  gdk_draw_line(pixmap,gris,//PRE-dibujamos
		0 , CERO0,
		B , CERO0);*/

  gdk_draw_line(pixmap,gris,//PRE-dibujamos
		0 , CERO0,
		B , CERO0);
  gdk_draw_line(pixmap,gris,//PRE-dibujamos
		0 , CERO1,
		B , CERO1);
  gdk_draw_line(pixmap,gris,//PRE-dibujamos
		0 , CERO2,
		B , CERO2);
  gdk_draw_line(pixmap,gris,//PRE-dibujamos
		0 , CERO3,
		B , CERO3);

}

/*Vamos a convrtir los puntos que nos manden, en coordenadas que
  puedan ser entendidas por el investigador (ed plano cartesiano)*/
void pintarPunto (gint x0,gint x1,gint x2,gint x3,  gpointer data){
  gint contador;
  gint k = 1;

    while (gtk_events_pending()) {
      gtk_main_iteration_do(0);
    }



  /*ACOMODAMOS EL BUFFER*/
    datosPantalla0[(B/k)-1] = A-x0-CERO3;
    datosPantalla1[(B/k)-1] = A-x1-CERO2;
    datosPantalla2[(B/k)-1] = A-x2-CERO1;
    datosPantalla3[(B/k)-1] = A-x3-CERO0;


    for(contador = 0; contador < (B/k)-1;contador++){

      gdk_draw_line(pixmap,negro, //PRE-dibujamos
		    k*contador , datosPantalla0[contador],
		    k*(contador+1) , datosPantalla0[contador+1]);
      datosPantalla0[contador] = datosPantalla0[contador+1];

      gdk_draw_line(pixmap,rojo, //PRE-dibujamos
		    k*contador , datosPantalla1[contador],
		    k*(contador+1) , datosPantalla1[contador+1]);
      datosPantalla1[contador] = datosPantalla1[contador+1];

      gdk_draw_line(pixmap,azul, //PRE-dibujamos
		    k*contador , datosPantalla2[contador],
		    k*(contador+1) , datosPantalla2[contador+1]);
      datosPantalla2[contador] = datosPantalla2[contador+1];

      gdk_draw_line(pixmap,verde, //PRE-dibujamos
		    k*contador , datosPantalla3[contador],
		    k*(contador+1) , datosPantalla3[contador+1]);
      datosPantalla3[contador] = datosPantalla3[contador+1];
    }
}

// t_tipo 0 : Continuar (Seguir con los ultimos datos)
// t_tipo 1 : Nuevo (Borrar pantalla)
// t_tipo 2 : Continuar con cambio de amplificacion
// t_tipo 2 : Continuar con cambio de frecuencia
void grafica_main(gpointer data, char *ruta, int t_tipo){
  //variables del socket
   time_t now,now2;
  struct tm *now_tm,*now_tm2;
  int hora,hora2;
  int sfd;
  int tuberia[5];
  int c;
  struct sockaddr_in ser_addr;
  Buffer buffer3; //Estructura para guardar datos!
  Buffer buffer2; //Estructura para guardar datos!
  Buffer buffer1; //Estructura para guardar datos!
  Buffer buffer0; //Estructura para guardar datos!
  FILE *destino3; //Archivo destino
  FILE *destino2; //Archivo destino
  FILE *destino1; //Archivo destino
  FILE *destino0; //Archivo destino
  GtkWidget* drawingarea = (GtkWidget *)data; //recibimos el area de dibujo
  gint i;
  gint m0,m1,m2,m3;
  GdkRectangle actualiza;  //Estructura para iniciar la pantalla en un color
  gint leidos;
  gint media1,media2,media3,media4,media;
  gint MEDIA = 10;
  char ruta1[100];
  char ruta2[100];  
  char ruta3[100];  
  char ruta4[100];
  static gint MemoriaPermanente[(1000+1)*4];

  media1=media2=media3=media4=media=0;

  /*Apertura de una conexion*/
  if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1){
    exit(-1);
  }

  /*Peticion de conexion con el servidor*/
  ser_addr.sin_family = AF_INET;
  ser_addr.sin_addr.s_addr=inet_addr(DIRECCION_NODO_SERVIDOR);
  ser_addr.sin_port = htons(PUERTO_SERVIDOR_TCP);

  if(connect(sfd,(const struct sockaddr *)&ser_addr,sizeof(ser_addr))==-1){
    exit(-1);
  }

  /*Comunicando con el servidor (Enviando configuracion)*/
  leidos = write(sfd,Memoria,sizeof(int)*20);
  if(leidos ==-1){
    exit(-1);
  }

  //Area de descarga de datos
  buffer3 = nuevoBuffer(20);
  buffer2 = nuevoBuffer(20);
  buffer1 = nuevoBuffer(20);
  buffer0 = nuevoBuffer(20);
  sprintf(ruta1,"%s/canal0.xdat",ruta); //Polarizacion
  sprintf(ruta2,"%s/canal1.xdat",ruta); //Intensidad
  sprintf(ruta3,"%s/canal2.xdat",ruta);
  sprintf(ruta4,"%s/canal3.xdat",ruta);
  //  g_print("%s,%s,%s,%s",ruta1,ruta2,ruta3,ruta4);


  if (CERO0==-1){
    B = drawingarea->allocation.width;
    A = drawingarea->allocation.height; //altura
    //    g_print("A=%i",A);
    //g_print("B=%i",B);
    CERO0 = A/2 -150;
    CERO1 = CERO0 + 100;
    CERO2 = CERO1 + 100;
    CERO3 = CERO2 + 100;
  }


    datosPantalla0 = (gint*)malloc(sizeof(gint)*(B+1)); //DATOS
    datosPantalla1 = (gint*)malloc(sizeof(gint)*(B+1)); //DATOS
    datosPantalla2 = (gint*)malloc(sizeof(gint)*(B+1)); //DATOS
    datosPantalla3 = (gint*)malloc(sizeof(gint)*(B+1)); //DATOS

    time(&now);  //Pido la hora
    now_tm = gmtime(&now); //la hora la transforma a UTC
    hora =hora2int(now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec);   

    /*Avisamos al buffer para que registre el inicio de la catura*/
    if((t_tipo == 1) || (t_tipo == 0)){
      c = Memoria[4];
      //Iniciamos a esta hora!
      agregarABuffer(&buffer3, 2, 0,0,hora);
      agregarABuffer(&buffer2, 2, 0,0,hora);
      agregarABuffer(&buffer1, 2, 0,0,hora);
      agregarABuffer(&buffer0, 2, 0,0,hora);
      //La amplificacion con la que iniciamos
      agregarABuffer(&buffer3, 2, 2, c, hora);
      agregarABuffer(&buffer2, 2, 2, c, hora);
      agregarABuffer(&buffer1, 2, 2, c, hora);
      agregarABuffer(&buffer0, 2, 2, c, hora);
      //La frecuencia con la que iniciamos
      c = Memoria[5];
      agregarABuffer(&buffer3, 2, 3, c, hora);
      agregarABuffer(&buffer2, 2, 3, c, hora);
      agregarABuffer(&buffer1, 2, 3, c, hora);
      agregarABuffer(&buffer0, 2, 3, c, hora);

    }
    /*-----------*/
    if(t_tipo == 2){ //Cambio de amplificacion
      Memoria[27] = 0; //ok ya se realizo el cambio, refresco la variable.
      c = Memoria[4];
      //Agregamos al buffer el cambio de amplificacion
      agregarABuffer(&buffer3, 2, 2, c, hora);
      agregarABuffer(&buffer2, 2, 2, c, hora);
      agregarABuffer(&buffer1, 2, 2, c, hora);
      agregarABuffer(&buffer0, 2, 2, c, hora);
    }
    /*------------*/
    if(t_tipo == 3){ //Cambio de amplificacion
      Memoria[27] = 0; //ok ya se realizo el cambio, refresco la variable.
      c = Memoria[5];
      //Agregamos al buffer el cambio de amplificacion
      agregarABuffer(&buffer3, 2, 3, c, hora);
      agregarABuffer(&buffer2, 2, 3, c, hora);
      agregarABuffer(&buffer1, 2, 3, c, hora);
      agregarABuffer(&buffer0, 2, 3, c, hora);
    }
    /*------------*/


    if(t_tipo == 1){     //Si el proyecto es nuevo iniciamos todo.
      destino3 = fopen(ruta4,"wb"); //destino
      destino2 = fopen(ruta3,"wb"); //destino
      destino1 = fopen(ruta2,"wb"); //destino
      destino0 = fopen(ruta1,"wb"); //destino
      for (i = 0; i <= B; i++){
	datosPantalla0[i] = CERO0;
	datosPantalla1[i] = CERO1;
	datosPantalla2[i] = CERO2;
	datosPantalla3[i] = CERO3;	
      }
    }else { //Si el proyecto existe continuamos con la captura o es un cambio de amplificacion o es de frecuencia
      destino3 = fopen(ruta4,"ab"); //destino
      destino2 = fopen(ruta3,"ab"); //destino
      destino1 = fopen(ruta2,"ab"); //destino
      destino0 = fopen(ruta1,"ab"); //destino     
      for (i = 0; i <= B; i++){
	datosPantalla0[i] =  MemoriaPermanente[(0*B)+i];
      }
      for (i = 0; i <= B; i++){
	datosPantalla1[i]= MemoriaPermanente[(1*B)+i];
      }
      for (i = 0; i <= B; i++){
	datosPantalla2[i]= MemoriaPermanente[(2*B)+i];
      }
      for (i = 0; i <= B; i++){
	datosPantalla3[i] = MemoriaPermanente[(3*B)+i];
      }      
    }

  //Señal utilizada para manejar el mapa de pixeles de segundo plano
   /*CREAMOS EL PRIMER CUADRO*/
  actualiza.x = actualiza.y = 0;
  actualiza.width = B;
  actualiza.height = A;
  if (pixmap == NULL){
    pixmap =gdk_pixmap_new(drawingarea->window,
			   drawingarea->allocation.width,
			   drawingarea->allocation.height,
			   -1);
    //llamamos rutina de repintado
    gdk_draw_rectangle(pixmap,drawingarea->style->white_gc,TRUE,0,0,B, A);
    gtk_widget_draw(drawingarea, &actualiza);
  }
  if (rojo==NULL){
    rojo = GetPen(NewColor(0xFFFF,0,0));
    verde =  GetPen(NewColor(0,0xFFFF,0));
    azul =  GetPen(NewColor(0,0,0xFFFF));
    negro =  GetPen(NewColor(0,0,0));
    gris =  GetPen(NewColor(0xDD00,0xDD00,0xDD00));
    transparente =  GetPen(NewColor(0xEE00,0xEE00,0xEE00));
  }

  /*TERMINA CREAMOS EL PRIMER CUADRO*/

  /*Rutina de pintado*/
  while(Memoria[1]){
    leidos = read(sfd,tuberia,sizeof(int)*5);
    if (leidos ==-1){
      exit(-1);
    }

    if(tuberia[4] != 0){ //hubo un error!!
      m3=m2=m1=m0=0; 
      g_print("\nError en la tarjeta LabPC+");
    }else {
    m3 =tuberia[2];
    m2 = tuberia[1];
    m1 = (-1)*tuberia[0]; //Intensidad
    m0 = tuberia[3]; //Polarizacion
    g_print("\n%i\n",m0); /*-------------------P R U E B A------------------------*/
    }

    media1 += m0;
    media2 += m1;    
    media3 += m2;
    media4 += m3;
    media++;

    if (media == MEDIA){ //Rutina para guardar la media.
      if(agregarABuffer(&buffer3, 1, 0, 0x3FFF & (int)(media4/MEDIA), 0) == 0) { //esta lleno
	guardarBuffer(buffer3,destino3,1); //binario
	vaciarBuffer(&buffer3);
	agregarABuffer(&buffer3, 1, 0,0x3FFF & (int)(media4/MEDIA), 0);
	guardarBuffer(buffer2,destino2,1); //binario
	vaciarBuffer(&buffer2);
	agregarABuffer(&buffer2, 1, 0, 0x3FFF & (int)(media3/MEDIA), 0);
	guardarBuffer(buffer1,destino1,1); //binario
	vaciarBuffer(&buffer1);
	agregarABuffer(&buffer1, 1, 0, 0x3FFF & (int)(media2/MEDIA), 0);
	guardarBuffer(buffer0,destino0,1); //binario
	vaciarBuffer(&buffer0);
	agregarABuffer(&buffer0, 1, 0,0x3FFF & (int)(media1/MEDIA), 0);
      }else{
	agregarABuffer(&buffer2, 1, 0, 0x3FFF & (int)(media3/MEDIA), 0);
	agregarABuffer(&buffer1, 1, 0, 0x3FFF & (int)(media2/MEDIA), 0);
	agregarABuffer(&buffer0, 1, 0, 0x3FFF & (int)(media1/MEDIA), 0);
      }
      //g_print("M1=%i,M2=%i,M3=%i,M4=%i;",media1/MEDIA,media2/MEDIA,media3/MEDIA,media4/MEDIA);
      media1=media2=media3=media4=media=0;
    } //fin de media

    if(agregarABuffer(&buffer3, 0, 0, (0x0FFF & m3), 0) == 0) { //esta lleno
      guardarBuffer(buffer3,destino3,1); //binario
      vaciarBuffer(&buffer3);
      agregarABuffer(&buffer3, 0, 0, (0x0FFF & m3), 0);
      guardarBuffer(buffer2,destino2,1); //binario
      vaciarBuffer(&buffer2);
      agregarABuffer(&buffer2, 0, 0, (0x0FFF & m2), 0);
      guardarBuffer(buffer1,destino1,1); //binario
      vaciarBuffer(&buffer1);
      agregarABuffer(&buffer1, 0, 0, (0x0FFF & m1), 0);
      guardarBuffer(buffer0,destino0,1); //binario
      vaciarBuffer(&buffer0);
      agregarABuffer(&buffer0, 0, 0, (0x0FFF & m0), 0);
    }else{
      agregarABuffer(&buffer2, 0, 0, (0x0FFF & m2), 0);
      agregarABuffer(&buffer1, 0, 0, (0x0FFF & m1), 0);
      agregarABuffer(&buffer0, 0, 0, (0x0FFF & m0), 0);
    }

    gdk_draw_rectangle(pixmap,drawingarea->style->white_gc,TRUE,0,0,B, A);
    //Pintamos escala
    escala(drawingarea);
     //Pintamos puntos

    pintarPunto((gint)m0,(gint)m1,(gint)m2,(gint)m3,drawingarea);
    //Desplegamos en pantalla
    //    actualiza.x = actualiza.y = 0;
    //actualiza.width = 600;
    //actualiza.height = 600;
    gtk_widget_draw(drawingarea, &actualiza);      
  }
  //  horaFin = time(NULL);
  //  g_print("\n%ld\n",horaFin-horaInicio);

  for (i = 0; i <= B; i++){
    MemoriaPermanente[(0*B)+i]=datosPantalla0[i];
  }
  for (i = 0; i <= B; i++){
    MemoriaPermanente[(1*B)+i]=datosPantalla1[i];
  }
  for (i = 0; i <= B; i++){
    MemoriaPermanente[(2*B)+i]=datosPantalla2[i];
  }
  for (i = 0; i <= B; i++){
    MemoriaPermanente[(3*B)+i]=datosPantalla3[i];
  }

    
  free(datosPantalla0);
  free(datosPantalla1);
  free(datosPantalla2);
  free(datosPantalla3);
  //cerrando conexion

  time(&now2);  //Pido la hora
  now_tm2 = gmtime(&now2); //la hora la transforma a UTC
  hora2 =hora2int(now_tm2->tm_hour,now_tm2->tm_min,now_tm2->tm_sec);
  //Falta asegurarnos que el buffer no esta lleno!!

    if((Memoria[27] == 1) || (Memoria[27] == 2)){
      g_print("\nTermino por amplificacion o frecuencia.");
    }else{
      g_print("\nTermino Normal.");
      //Si No  terminamos por una llamada de cambio de amplificacion
      if(agregarABuffer(&buffer3, 2, 1,0,hora2) == 0) { //esta lleno
	guardarBuffer(buffer3,destino3,1); //binario
	vaciarBuffer(&buffer3);
	agregarABuffer(&buffer3, 2, 1,0, hora2);
	guardarBuffer(buffer2,destino2,1); //binario
	vaciarBuffer(&buffer2);
	agregarABuffer(&buffer2, 2, 1, 0, hora2);
	guardarBuffer(buffer1,destino1,1); //binario
	vaciarBuffer(&buffer1);
	agregarABuffer(&buffer1, 2, 1, 0, hora2);
	guardarBuffer(buffer0,destino0,1); //binario
	vaciarBuffer(&buffer0);
	agregarABuffer(&buffer0, 2, 1,0, hora2);
      }else{
	agregarABuffer(&buffer2, 2, 1,0,hora2); //seguimos grabando
	agregarABuffer(&buffer1, 2, 1,0,hora2);
	agregarABuffer(&buffer0, 2, 1,0,hora2);
      }
    }
    Memoria[0] = 0; //decimos que ya realizamos el cambio.
    guardarBuffer(buffer3,destino3,1); //binario
    vaciarBuffer(&buffer3);
    guardarBuffer(buffer2,destino2,1); //binario
    vaciarBuffer(&buffer2);
    guardarBuffer(buffer1,destino1,1); //binario
    vaciarBuffer(&buffer1);
    guardarBuffer(buffer0,destino0,1); //binario
    vaciarBuffer(&buffer0);
    borrarBuffer(&buffer3);
    borrarBuffer(&buffer2);  
    borrarBuffer(&buffer1);  
    borrarBuffer(&buffer0);
    close(sfd);  
    fclose(destino3);
    fclose(destino2);
    fclose(destino1);
    fclose(destino0);
}
