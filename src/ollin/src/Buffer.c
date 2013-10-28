/*
* $Id$
*
* Buffer.c for Linux Version 0.8.5
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
* Buffer is a generic interfaz to allow data in Buffer Structure.
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005 No bugs now.
* ###############################################################
*/

#include<stdio.h>
#include<stdlib.h>
#include<Objeto.h>

Buffer nuevoBuffer(int tamano){
  Buffer buffer;
  buffer.datos = (char *)malloc(5*sizeof(char)*tamano+1); //la estructura
                                                        //maxima es de 5 bytes
  buffer.tamano = tamano;
  buffer.indice = 0;  //el indice del buffer de 1 en 1 (byte X byte)
  buffer.actual = 0;  //El indice de la estructura! 
  return buffer;
}

/* regresa 1 para verdadero*/
int bufferLleno(Buffer buffer){
  if (buffer.actual == buffer.tamano) return 1; //verdadero
  else
    return 0;
}

/*Regresa 0 si esta lleno */
/* modo 0 dato
        1 media
        2 cambio de estado
Para dato y media : tipo = hora = 0;
para cambio de estado
        tipo: 0 INICIO
              1 FIN
              2 CAMBIO DE AMPLIFICACION
              3 CAMBIO DE FRECUENCIA
              4 INICIA EVENTO
              5 TERMINA EVENTO
              6 DATOS PARA LA MEDIA
              7 VALOR DE EPSILON
              8 NUMERO DE EPSILONS
              9 EDO DE CALIBRACION

Representacion simbolica binaria
modo
0 -> 00XXDDDD DDDDDDDD
1 -> 01DDDDDD DDDDDDDD
2 -> 10TTTTTD DDDDDDDD DDDDDDDF FFFFFFFF FFFFFFFF
     87654321 87654321 87654321 87654321 87654321
Donde X no importa
      D {0,1} Que representan el dato
      T {0,1} Que representan el tipo de cambio
      F {0,1} Que representan la hora en el sig formato
              T HHHHMMMM MMSSSSSS
              T 0 = AM
                1 = PM
                H = {0,1} horas (1..12)
                M = {0,1} minutos (0..59)
                S = {0,1} segundos (0..59)
*/
   
int agregarABuffer(Buffer *buffer, int modo, int tipo,int dato, int hora){
  //  int empaquetador = 0;  
  int mask;

  
  
  if (bufferLleno(*buffer)) return 0; 
  else{
    buffer->actual++; //anadimos otro    
    switch(modo){
    case 0:
      mask = 0x0F00; //0000 1111 0000 0000 
      //00XXDDDD
      buffer->datos[buffer->indice] = (char)((dato & mask) >> 8); // 0000 1111
      mask = 0x0FF;    // 0000 1111 1111
      buffer->indice++;      
      buffer->datos[buffer->indice] = (char)(dato & mask); // 1111 1111
      buffer->indice++;      //ok
      return 1;
      
    case 1:
      mask = 0x3F00; //0011 1111 0000 0000
      //01DDDDDD
      buffer->datos[buffer->indice] = (char)(((dato & mask) >> 8) | 0x40); //ponemos 01
      buffer->indice++;                                           // 0111 1111
      mask = 0x0FF;    //0000 1111 1111
      buffer->datos[buffer->indice] = (char)(dato & mask); // 1111 1111
      buffer->indice++;
      return 1;
      
    case 2:
      buffer->datos[buffer->indice] = (char)((0x02 << 6) | ((tipo & 0x01F) << 1) | ((dato & 0x8000) >> 15)) ;
      buffer->indice++;
      buffer->datos[buffer->indice] = (char)((dato & 0x7F80) >> 7);
      buffer->indice++;
      buffer->datos[buffer->indice] = (char)(((dato & 0x7F) << 1) | ((hora & 0x10000) >> 16));
      buffer->indice++;
      buffer->datos[buffer->indice] = (char)((hora & 0x0FF00) >> 8);
      buffer->indice++;
      buffer->datos[buffer->indice] = (char)(hora & 0x0FF);
      buffer->indice++;
      /*
      mask = 0x01F;  // 0000 0001 1111 
      mask2 = 0x8000; // 1000  0000 0000 0000     
      //10TTTTTD                           ??TTTTT?           10??????       ???????D      
      op1 = (tipo & mask) << 1;
      //      printf("op1 %u\n", op1);  
      op2 = (dato & mask2) >> 15;
      //printf("op2 %u\n", op2);
      op3 = (op1 | op2);
      //printf("op3 %u\n", op3);
      op4 = (op3 | 0x80);
      paquete = op4;      
      //printf("op4 %u\n", op4);
      //printf("op4 %i\n", paquete);
      buffer->datos[buffer->indice] = paquete;
      //      printf("%u", buffer->datos[buffer->indice]);
      buffer->indice++;
      mask = 0x7F80;              //    (0000111111110000000 >> 7) => 00000000 11111111
      buffer->datos[buffer->indice] = (dato & mask) >> 7;
      buffer->indice++;
      mask =0x7F;                 //      000000001111111 << 1 => 0001111111?
      mask2 = 0x10000;
      buffer->datos[buffer->indice] = ((dato & mask) << 1) | ((hora & mask2) >> 16);
      buffer->indice++;
      mask = 0xFF00;        //           0001111111100000000 >> 8 => 0000000111111111
        buffer->datos[buffer->indice] = (hora & mask) >> 8;
        buffer->indice++;
      mask = 0xFF;        //           00011111111
        buffer->datos[buffer->indice] = (hora & mask);
        buffer->indice++;*/
      return 1;
    }
  }
}

void vaciarBuffer(Buffer *buffer){
  buffer->indice = 0;
  buffer->actual = 0;
}

void borrarBuffer(Buffer *buffer){
  free(buffer->datos); 
  buffer->indice = 0;
  buffer->tamano = 0;
  buffer->actual = 0;  
}

/*Buffer cambiarTamano(Buffer buffer);*/
int agregarChars(Buffer *buffer, int modo, char *datos){
  if (bufferLleno(*buffer)) return 0; 
  else{
    buffer->actual++; //anadimos otro    
    if (modo == 0){
      buffer->datos[buffer->indice] = datos[0];
      buffer->indice++;      
      buffer->datos[buffer->indice] = datos[1];
      buffer->indice++;      
      return 1;      
    } else {
      buffer->datos[buffer->indice] = datos[0];
      buffer->indice++;      
      buffer->datos[buffer->indice] = datos[1];
      buffer->indice++;
      buffer->datos[buffer->indice] = datos[2];
      buffer->indice++;      
      buffer->datos[buffer->indice] = datos[3];
      buffer->indice++;      
      buffer->datos[buffer->indice] = datos[4];
      buffer->indice++;      
      return 1;            
    }    
  }
  return 1;  
}


  

