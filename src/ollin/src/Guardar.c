/*
* $Id$
*
* Guardar.c for Linux Version 0.8.6
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
* Save Buffer data in xdat or txt format.
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005 Configuramos Amplificacion y Frecuencia.
* 0.8.6 Abril/2005 Importante Bug, se resolvio con un CAST en lec2
* ###############################################################
*/


#include<stdio.h> 
#include<stdlib.h>
#include<Guardar.h>
#include <unistd.h>

void guardarCabezera(FILE *archivo)
{
  fprintf(archivo,"<!-------------------------------------------------->\n");
  fprintf(archivo,"<!-- Por xquetzal                                 -->\n");
  fprintf(archivo,"<!--        Tabla de Codigos                      -->\n");
  fprintf(archivo,"<!-- modos:                                       -->\n");
  fprintf(archivo,"<!--      00: Dato                                -->\n");
  fprintf(archivo,"<!--      01: Media                               -->\n");
  fprintf(archivo,"<!--      02: Cambio de Estado:                   -->\n");
  fprintf(archivo,"<!--                    00: Inicio                -->\n");
  fprintf(archivo,"<!--                    01: Fin                   -->\n");
  fprintf(archivo,"<!--                    02: Amplificacion         -->\n");
  fprintf(archivo,"<!--                    03: Frecuencia            -->\n");
  fprintf(archivo,"<!--                    04: Evento (Inicia)       -->\n");
  fprintf(archivo,"<!--                    05: Evento (Termina)      -->\n");
  fprintf(archivo,"<!--                    06: Datos para la media   -->\n");
  fprintf(archivo,"<!--                    07: Datos para la Epsilon -->\n");
  fprintf(archivo,"<!--                    08: Numero de Epsilon     -->\n");
  fprintf(archivo,"<!--                    09: Estado de Calibracion -->\n");
  fprintf(archivo,"<!-------------------------------------------------->\n");
  fprintf(archivo,"<!-- Ejemplos                                     -->\n");
  fprintf(archivo,"<!-- 00 1234                                      -->\n");
  fprintf(archivo,"<!-- Dato = 1234                                  -->\n");
  fprintf(archivo,"<!-- 01 34                                        -->\n");
  fprintf(archivo,"<!-- Media = 34                                   -->\n");
  fprintf(archivo,"<!-- 02 02 3 12:10:33                             -->\n");
  fprintf(archivo,"<!-- Cambio de Estado, Amplificacion, con un valor-->\n");
  fprintf(archivo,"<!-- de 3 hecho a las 12:10:33                    -->\n");
  fprintf(archivo,"<!-------------------------------------------------->\n");
}


int convierteHora(int ampm, int hora, int min, int seg){
  int tiempo = 0;
  tiempo = (0x01 & ampm) << 16;
  tiempo = tiempo | (0x0F & hora) << 12;
  tiempo = tiempo | (0x3F & min) << 6;
  tiempo = tiempo | (0x3F & seg);
  return tiempo;  
}

int guardarBuffer(Buffer buffer, FILE *archivo, int modoDGuardar){ //modoDGuardar::= 0->ASCII | 1->Binario.
  int tam;
  //int modo;   
  int i,lec0,lec1,lec2,lec3,lec4;  
  int res; /*aqui falta para c2*/

  char z;
  if (modoDGuardar){  //==1
    tam = buffer.indice;
    for(i = 0; i < tam; i++){  //aqui es -1
      fwrite(&(buffer.datos[i]),sizeof(char),1,archivo);      
    }
    fflush(archivo);
    return 1;
  }else if (modoDGuardar == 0){
    /*Guardando en modo texto*/
    int index=0;
    while(index < buffer.indice-1){ //mientra hay datos
      z=buffer.datos[index]; //leo un caracter!
      //0xC0 1100 0000
      switch ( (z&0xC0) >>6){ //Leo el modo: 0 = dato; 1 = media; 2 = cambio
      case 0 : //es un dato!  00XXDDDD DDDDDDDD
	lec2 = z&0x0F; //4 primeros digitos 0000 1111
	lec1 = 0x0FF & buffer.datos[++index]; // 8 segundos digitos
	if ((lec2 & 0x08) == 8) { //es negativo  0000 1000
	  lec2 = (0xFFFFF000 | (lec2 << 8));
	}else{
	  lec2 = (0x00000FFF & (lec2 << 8)); //es positivo
	}
	res = (lec1 | lec2);
	fprintf(archivo,"00 %i\n",res);
	index++;      
	break;      
      case 1 : //es Media!  01DDDDDD DDDDDDDD
	lec2 = z&0x3F; //6 primeros digitos 0011 1111
	lec1 = 0x0FF &buffer.datos[++index]; // 8 segundos digitos
	if ((lec2 & 0x020) == 32){ //es negativo
	  lec2 = (0xFFFFC000 | (lec2 << 8)); // 1100 0000 0000 0000
	}else{
	  lec2 =  (0x00003FFF & (lec2 << 8)); //es positivo
	}
	res = (lec1 | lec2);
	fprintf(archivo,"01 %i\n",res);
	index++;
	break;
      case 2 : //es Cambio de Estado!  10DDDDDD DDDDDDDD
	lec0 = z;
	lec1 = 0x0FF &buffer.datos[++index];
	lec2 = 0x0FF &buffer.datos[++index];
	lec3 = 0x0FF &buffer.datos[++index];
	lec4 = 0x0FF &buffer.datos[++index];
	index++;
	fprintf(archivo,"02, %i, ", ((lec0 & 0x3E) >> 1)); //pinto el modo y el tipo	
	fprintf(archivo," %i, ", ((lec0 & 0x1) << 15) | ((lec1& 0xFF) << 7) | ((lec2 & 0xFE) >> 1));//dato
	if(lec2 & 0x01) fprintf(archivo,"%i:",((lec3 & 0xF0) >> 4) + 12); //pm
	else
	  fprintf(archivo,"%i:",(lec3 & 0xF0) >> 4); //am
	fprintf(archivo,"%i:",((lec3 & 0x0F) << 2) | ((lec4 & 0xC0) >> 6)); //minutos
	fprintf(archivo,"%i\n",lec4 & 0x3F);
	break;
      }
    }
    fflush(archivo);
    return 1;
  }
  return 0;  
}  //Termina guardaBuffer.

int guardarCola(Cola cola, FILE *archivo, int modoDGuardar){  //modoDGuardar::= 0->ASCII | 1->Binario.
  return 0;
}


