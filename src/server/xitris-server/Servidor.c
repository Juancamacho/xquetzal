/*
* $Id$
*
* Server.c for Linux Version Alfa (0.9)
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
* Deamon to copy files when this change it size.
* 
* ###############################################################
*
* ###################### Revision History #######################
*
* $Log$
* 0.8.5 Feb/2005
* ALFA Mayo/2005 Cambio de Frecuencias de muestreo.
* ###############################################################
*/

#include "i_addr.h"
#include "LabPC.h"
#include <wait.h> 

int configuracion[20];
int kpasos;


void recoge_hijos()
  {
    int wstatus;
    (void)wait(&wstatus);
  }


void
configurar(){
  int conf;
  int Byte;

  conf = configuracion[4];
  Byte =  (int)(((conf & 0x07) << 4) | (0x01 << 3) | 0x03);
  commandRegister1((char)Byte);
  //Escribiendo la misma configuracion pero con un bit al principio.
  //    Byte = 139;
  //      scanen               Gain                         TWOSCMP          //         1              X X X                           1           011
  Byte =  (int)(0x080 | ((conf & 0x07) << 4) | (0x01 << 3) | 0x03);
  printf("\nAmplificacion = %i \n Frecuencia = %i",configuracion[4],configuracion[5]);
  commandRegister1((char)Byte);
  
  //CHANNEL SCANNING
  Byte = 12;  //00001100
  commandRegister4((char)Byte);
  
  //INTERVAL SCANNING
  //Byte = 13; //00001101
  //commandRegister4((char)Byte);

  Byte = 48; //00110000
  commandRegister2((char)Byte);
  Byte = 0x34;
  //Preparando el reloj
  counterAModeRegister((char)Byte);

  /*esto es lo importante de la frecuencia*/
  /*
   *
   */


  switch(configuracion[5]){

  case 0:
    //Byte menos significativo
    Byte = 0xFA;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x00;
    counterA0DataRegister((char)Byte);

    //printf("\nAmplificacion = %i\n",configuracion[4]);
    kpasos=0;
    break;


  case 1:
    /* Para .01 configuracion[5] == 1
     *  es decir 0000 1000 1110 001 (1248)
     * 0x02710/4 = 0x9CA
     * kpasos= 0
     * OK!
     */
    //Byte menos significativo
    Byte = 0xC4;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x09;
    counterA0DataRegister((char)Byte);
    kpasos=0;
    break;

  case 2: 
    /* Para .5 configuracion[5] == 2
     * x=5000_{10} =  0x1388
    */
    //Byte menos significativo
    Byte = 0x88;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x13;
    counterA0DataRegister((char)Byte);
    kpasos = 25;
    break;
    /*estan listos*/

  case 3:
    /* Para 1 configuracion[5] == 3
     *
     */
    //Byte menos significativo
    Byte = 0x88;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x13;
    counterA0DataRegister((char)Byte);
    kpasos = 50;
    break;

  case 4:
    /* Para 2 configuracion[5] == 4
     */
    //Byte menos significativo
    Byte = 0x88;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x13;
    counterA0DataRegister((char)Byte);
    kpasos = 100;
    break;

  case 5:
    /* Para 5 configuracion[5] == 5
     */
    //Byte menos significativo
    Byte = 0x88;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x13;
    counterA0DataRegister((char)Byte);
    kpasos = 250;
    break;

  case 6:
    /* Para 10 configuracion[5] == 6
     */
    //Byte menos significativo
    Byte = 0x88;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x13;
    counterA0DataRegister((char)Byte);
    kpasos = 500;
    break;

  case 7:
    /* Para 30 configuracion[5] == 6
     */
    //Byte menos significativo
    Byte = 0x88;
    counterA0DataRegister((char)Byte);
    //Byte mas significativo
    Byte = 0x13;
    counterA0DataRegister((char)Byte);
    kpasos = 1500;
    break;


  }//final del switch
  /*
   *
   */

  Byte = 0x70;
  counterAModeRegister((char)Byte);

  //  outb(0x00,BASEPORT+0x08);
  Byte = 0x00;
  ADClearRegister((char)Byte); //Reseteamos para iniciar
  ADFIFOregister();
  Byte = 52; //00110100
  commandRegister2((char)Byte);

}


main (int argc, char *argv[]){
  int SALIR;
  int sfd, nsfd, pid,estado;
  char buffer[2];
  struct sockaddr_in ser_addr, cli_addr;
  int cli_addr_len;
  int tarjeta[5];
  int bw,br,ok;
  int i;

  //  nombre_prog = argv[0];

  signal(SIGCHLD, recoge_hijos); /* para evitar zombies */
  
  
    if((sfd = socket(AF_INET,SOCK_STREAM,0))==-1){ //abrimos un socket
      printf("\nError al abrir el Socket.\n");
      exit (-1);
    }
    ser_addr.sin_family = AF_INET; //Definimos el puerto y lo publicamos
    ser_addr.sin_addr.s_addr=inet_addr(DIRECCION_NODO_SERVIDOR);
    ser_addr.sin_port = htons(PUERTO_SERVIDOR_TCP);
    if(bind(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) == -1){
      printf("\nEn la Publicidad del puerto (Bind).\n");
      exit(-1);
    }


      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nXquetzal Server (ALFA)\n");
    printf("Esperando...\n");  

    listen(sfd,1); //Decimos que solo esperamos a 1 cliente    
    printf(".");
    cli_addr_len = sizeof(cli_addr);

    while(1){    
    if((nsfd = accept(sfd,(struct sockaddr *)&cli_addr,&cli_addr_len))==-1){
      printf("\nError al Resolver Peticion.\n");
      exit(0);
    }    


        if(fork()==0)
          {
            /* hijo */
            /* usar socket t aquí */
	    // printf("\nConexion Abierta.\n");
	    //Leyendo configuracion
	    //    printf("Recibiendo configuracion de xquetzal.\n");
	    read(nsfd, configuracion,sizeof(int)*20);
	    
	    if(!tarjetaDisponible()){
	      printf("\nError al establecer conexion con la tarjeta decodificadora.\n");
	      exit(0);
	    }
	    //4 = Amplificacion.
	    printf("Configurando Lab PC+.\n");
	    iniciaTarjeta(); /*Iniciamos la tarjeta*/
	    configurar(); //configuramos la tarjeta
	    bw=0;
	    //printf("\nEnviando Datos...");
	    SALIR=0;
	    tarjeta[4] = 0;
	    
	    while(bw != -1){
	      i=0;

	      

      /*-INICIO>------------------- Capturando para k-pasos---------------------------*/
      do{
	i++;
	datoListo();
	if ((tarjeta[0]= ADFIFOregister()) == 5000){ //hubo un overflow
	  tarjeta[0] = 0;
	  tarjeta[4] = 1;  //Avisamos que no hay errores
	  i=kpasos;
	  SALIR=1;
	}else if(tarjeta[0] == -5000){ //hubo un underflow
	  tarjeta[0] = 0;
	  tarjeta[4] = -1; //Hubo un error, este entero esta en -1 para anunciar un error.
	  i=kpasos;
	  SALIR=1;
	}	

	if(!SALIR){
	  datoListo();
	  if ((tarjeta[1]= ADFIFOregister()) == 5000){ //hubo un overflow
	    tarjeta[1] = 0;
	    tarjeta[4] = 1;
	    i=kpasos;
	    SALIR=1;
	  }else if(tarjeta[1] == -5000){ //hubo un underflow
	    tarjeta[1] = 0;
	    tarjeta[4] = -1;
	    i=kpasos;
	    SALIR=1;
	  }	  

	  if(!SALIR){
	    datoListo();
	    if ((tarjeta[2]= ADFIFOregister()) == 5000){ //hubo un overflow
	      tarjeta[2] = 0;
	      tarjeta[4] = 1;
	      i=kpasos;
	      SALIR=1;
	    }else if(tarjeta[2] == -5000){ //hubo un underflow
	      tarjeta[2] = 0;
	      tarjeta[4] = -1;
	      i=kpasos;
	      SALIR=1;
	    }

	    if(!SALIR){	    
	      datoListo();
	      if ((tarjeta[3]= ADFIFOregister()) == 5000){ //hubo un overflow
		tarjeta[3] = 0;
		tarjeta[4] = 1;
		i=kpasos;	   
	      }else if(tarjeta[3] == -5000){ //hubo un underflow
		tarjeta[3] = 0;
		tarjeta[4] = -1;
		i=kpasos;	   
	      }
	    }//ultimo NO SALIR
	  }//Segundo NO SALIR
	} //primer NO SALIR	
      }while(i < kpasos);
      /*-FIN>---------------------- Capturando para k-pasos---------------------------*/
      //      if (nsfd != NULL)


      //printf("nsfd = %i &nsfd= %i \n",nsfd,&nsfd);

      bw=write(nsfd,tarjeta,sizeof(int)*5);

	//      else
	//bw = -1;

      //printf("bw = %i\n",bw);
      if(bw == -1){               //Mandando los bits!
	//printf("\nError al enviar datos.\n");                       //Un error
	close(nsfd);
	bw=-1;       
      }else if(bw < 20){
	//printf("\nNo se enviaron datos.\n");
	close(nsfd);
	bw=-1;
      }else{                                                      //No hubo un error.
	//printf("Datos Enviados: %i \n",bw);
	bw = 0;
      }


      /*--------------- Hubo un error, hay que reconfigurar--------------------*/
      if(SALIR){
	printf("SALIR");
	iniciaTarjeta(); /*Iniciamos la tarjeta*/
	configurar();
      }


      tarjeta[0]=tarjeta[1]=tarjeta[2]=tarjeta[3]=tarjeta[4] = 0; //Limpiamos canales.
      SALIR =0;                                                   //Limpiamos errores para continuar.

    }                                                            //while (bw != -1)

    //    close(nsfd);  
	  }
        else
          {
            /* padre */
	    //  printf("\n\nCerrando Conexion\n");
            close(nsfd);
          }
    }
}
