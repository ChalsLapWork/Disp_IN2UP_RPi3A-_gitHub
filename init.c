#include <wiringPi.h>
#include "system.h"
#include "init.h"
#include <stdio.h>
#include "errorController.h"

// Estructura para manejar los pines y datos
typedef struct {
    int pins[8];      // Pines GPIO para datos
} ParallelPort;

ParallelPort port = { .pins = {0, 1, 2, 3, 4, 5, 6, 7} };
void initParallelPort(ParallelPort *port);


// Configurar pines como salidas
void initParallelPort(ParallelPort *port) {
    for (int i = 0; i < 8; i++) {
        pinMode(port->pins[i], OUTPUT);
        digitalWrite(port->pins[i], LOW); }
}//fin de init parallel port++++++++++++++++++++++++++++

void configPuertos(void){
  printf("\n       Started Configuracion de Puertos");
  /* pinMode(BIT0_PIN,OUTPUT);
  pinMode(BIT1_PIN,OUTPUT);
  pinMode(BIT2_PIN,OUTPUT);
  pinMode(BIT3_PIN,OUTPUT);
  pinMode(BIT4_PIN,OUTPUT);
  pinMode(BIT5_PIN,OUTPUT);
  pinMode(BIT6_PIN,OUTPUT);  */
  initParallelPort(&port);
  pinMode(WR_PIN  ,OUTPUT);
  pinMode(R_BUSY_PIN,INPUT);
#if (debug_level1==1) 
   //printf("\r\033[0;32m[ ok ]\033[0m");
    NoErrorOK();
#endif  
}//fin init puertos++++++++++++++++++++++++++++++++++++++++++++


void writePort(unsigned char value){
   digitalWrite(WR_PIN,LOW);//comando de escritura 
   writeParallelPort(&port,value);
   digitalWrite(WR_PIN,HIGH);//comando de escritura OFF
}//fin write port++++++++++++++++++++++++++++++++++++++++++


// Escribir un byte en el puerto paralelo +++++++++++++++++++++++
void writeParallelPort(ParallelPort *port, unsigned char value) {
    for (int i = 0; i < 8; i++) {
        int bit = (value >> i) & 1;       // Extraer el i-ésimo bit
        digitalWrite(port->pins[i], bit); }// Establecer el valor en el pin correspondiente
}//fin de write parallelport ++++++++++++++++++++++++++++++++++++



void init_Display(void){
// unsigned char init[]=0x00,0x00}; 
 //unsigned char delays={  1,  2};
    // display_data(&init[0],&delays[0])
 
}//fin de init_Display+++++++++++++++++++++