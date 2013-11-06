#ifndef LABPC_H
#define LABPC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BASEPORT 0x0260
  
/*
 * Revisa si hay permiso para acceder a el puerto
 * @return 1 si hay permiso
 */
int tarjetaDisponible( void );  

/*
 * Inicia la tarjeta para comenzar a recibir datos
 */
void iniciaTarjeta( void );  

int ADFIFOregister(void);
  
void empezarConversion(void);

void datoListo();

void commandRegister1(char var);

void commandRegister2(char var);

void commandRegister4(char var);

void counterAModeRegister(char var);

void counterA0DataRegister(char var);

  void ADClearRegister(char var);

  int leerReg();

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* BUFFER_H */

