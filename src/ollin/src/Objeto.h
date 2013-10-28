/*
* $Id$
*
* Objeto.h library for Linux Version 0.8.5
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
* Estructura para definir nuestra entidad minima de informacion
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005 Configuramos Amplificacion y Frecuencia.
* ###############################################################
*/


#ifndef OBJETO_H
#define OBJETO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _buffer{
  char *datos;
  int tamano;  //cantidad de datos a meter.
  int indice;  //indice en el arreglo.
  int actual;  //contador de datos.
}Buffer;

typedef struct _cola{
  char *datos;
  int tamano;
  int indice;
}Cola;


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* OBJETO_H */



