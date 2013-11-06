
/*
 * Server Simulator 0.9.1 (xquetzal 0.8.6)
 * Configuramos amplificacion y frecuencia!
 * Reconfiguramos Frecuencia!
 * Seno X
 */

#include "i_addr.h"
#include <wait.h> 

int configuracion[20];
int kpasos;
float sleeping;

void recoge_hijos()
  {
    int wstatus;
    (void)wait(&wstatus);
  }


void
configurar(){
 
  printf("\nAmplificacion = %i \n Frecuencia = %i",configuracion[4],configuracion[5]);

  switch(configuracion[5]){

  case 0:
    sleeping = 0.001;
    break;


  case 1:
    sleeping = 0.01;
  break;

  case 2: 
    sleeping = 0.5;
    break;  

  case 3:
    sleeping = 1;  
    break;

  case 4:
    sleeping = 2;     
    break;

  case 5:
    sleeping = 5;  
    break;

  case 6:
    sleeping = 10;         
    break;

  case 7:
    sleeping = 30;  
    break;


  }//final del switch

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

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nXquetzal Server Simulator 9.1\n");
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
	  read(nsfd, configuracion,sizeof(int)*20);
	  printf("Configurando Lab PC+.\n");
	  configurar(); //configuramos la tarjeta
	  bw=0;
	  SALIR=0;
	  tarjeta[4] = 0;
	  while(bw != -1){
	    tarjeta[0]= 1;
	    tarjeta[1]= 1;
	    tarjeta[2]= 1;
	    tarjeta[3]= 1;
	    bw=write(nsfd,tarjeta,sizeof(int)*5);
	    if(bw == -1){               //Mandando los bits!
	      //printf("\nError al enviar datos.\n"); //Un error
	      close(nsfd);
	      bw=-1;       
	    }else if(bw < 20){
	      //printf("\nNo se enviaron datos.\n");
	      close(nsfd);
	      bw=-1;
	    }else{//No hubo un error.
	      //printf("Datos Enviados: %i \n",bw);
	      bw = 0;
	    }
  
	    if(SALIR){
	      printf("SALIR");
	      configurar();
	    }
		    
	    tarjeta[0]=tarjeta[1]=tarjeta[2]=tarjeta[3]=tarjeta[4] = 0; //Limpiamos canales.
	    SALIR =0;     //Limpiamos errores para continuar.
	    sleep(1);
	  }//while (bw != -1)

	  
	}
      else
	{
	  /* padre */
	  //  printf("\n\nCerrando Conexion\n");
	  close(nsfd);
	}
    }
}
