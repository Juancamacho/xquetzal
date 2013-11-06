#include "LabPC.h" /* Cabezeras de la propia biblioteca */
#include <stdio.h>      /* Libraria estandard de C           */
#include <unistd.h>     /* ioperm()                          */
#include <sys/io.h>     /* outb(), intb()                    */


int tarjetaDisponible( void ){ 
  if (ioperm(BASEPORT, 25, 1))
    return 0;
  else
    return 1;    
}

/*
 * Inicia la tarjeta para comenzar a recibir datos
 */
void iniciaTarjeta( void ){
  unsigned lec1;
  unsigned lec2;
  outb(0x00,BASEPORT);           
  outb(0x00,BASEPORT+0x0001) ;
  outb(0x00,BASEPORT+0x0002) ;
  outb(0x00,BASEPORT+0x000F) ;
  outb(0x34,BASEPORT+0x0017) ;
  outb(0x0A,BASEPORT+0x0014) ;
  outb(0x00,BASEPORT+0x0014) ;
  outb(0x00,BASEPORT+0x000A) ;
  outb(0x00,BASEPORT+0x000C) ;
  outb(0x00,BASEPORT+0x0008) ;
  /* limpia el registro de salida FIFO*/
  lec1 = inb(BASEPORT+0x000A); 
  lec2 = inb(BASEPORT+0x000A);  
}


int ADFIFOregister(void){
  int lec1,lec2;
  int res; /*aqui falta para c2*/
  int MASK = 128;
  int MASKCLEAR = 0x000000FF;
  if ((0x004 & inb(BASEPORT)) >> 2){ //Error de Overflow!!
    return 5000;   //estos valores jamas son alcansados por la tarjeta
  }else if ((0x002 & inb(BASEPORT)) >> 1){ //Error de Overrrun!!
    return -5000; //estos valores jamas son alcansadospor la tarjeta
  }else{
    lec1= (MASKCLEAR & inb(BASEPORT+0x000A)); //byte bajo
    lec2= (MASKCLEAR & inb(BASEPORT+0x000A)); //byte alto
    if ( (lec2 & MASK) == 128){ //es negativo
      lec2 = (0xFFFFF000 | (lec2 << 8));
    }else{
      lec2 = (lec2 << 8); //es positivo
    }
    res = (lec1 | lec2);
    return res;
  }
}

void empezarConversion(void){
   outb(0x04,BASEPORT+0x01);
}
 
void datoListo(){
while(!(inb(BASEPORT) & 1)) /*inb(0x260)*/ ;
}

void commandRegister1(char var){
  outb(var,BASEPORT); /*commandregister1*/ 
}

void commandRegister2(char var){
  outb(var,BASEPORT + 0x01); /*commandregister1*/ 
}

void commandRegister4(char var){
  outb(var,BASEPORT+0x0F); /*commandregister4*/ 
}

void counterAModeRegister(char var){
  outb(var,BASEPORT+0x17);
}

void counterA0DataRegister(char var){
  outb(var,BASEPORT+0x14);
}

void ADClearRegister(char var){
  outb(var,BASEPORT+0x08);
}

int leerReg(){
  return inb(BASEPORT+0x000A); 
}
