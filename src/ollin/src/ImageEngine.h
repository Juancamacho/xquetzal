/*
* $Id$
*
* ImageEngine.h Library for Linux Version 0.8.5
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
* Image Engine for xquetzal client.
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005 Configuramos Amplificacion y Frecuencia.
* ###############################################################
*/

#include <gtk/gtk.h>

#ifndef IMAGEENGINE_H
#define IMAGEENGINE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void grafica_main(gpointer data, char *ruta, int t_tipo); 

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* BUFFER_H */
