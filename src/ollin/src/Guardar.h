/*
* $Id$
*
* Guardar.h Library for Linux Version 0.8.5
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
* Save Buffer data into xdat or txt format.
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005
* ###############################################################
*/

#include<stdio.h>
#include<stdlib.h>
#include<Objeto.h>
 

#ifndef GUARDAR_H
#define GUARDAR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int guardarBuffer(Buffer buffer, FILE *archivo, int tipo);
int guardarCola(Cola buffer, FILE *archivo, int tipo);
void guardarCabezera(FILE *archivo);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* GUARDAR_H */













