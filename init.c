#include <wiringPi.h>
#include "system.h"
#include "init.h"
#include <stdio.h>
#include "errorController.h"
#include "VFD.h"

void configPuertos(void){
  printf("\n       Started Configuracion de Puertos");
  /*pinMode(BIT0_PIN,OUTPUT);
    pinMode(BIT1_PIN,OUTPUT);
    pinMode(BIT2_PIN,OUTPUT);
    pinMode(BIT3_PIN,OUTPUT);
    pinMode(BIT4_PIN,OUTPUT);
    pinMode(BIT5_PIN,OUTPUT);
    pinMode(BIT6_PIN,OUTPUT);*/
  
  pinMode(WR_PIN  ,OUTPUT);
  pinMode(R_BUSY_PIN,INPUT);
#if (debug_level1==1) 
   //printf("\r\033[0;32m[ ok ]\033[0m");
    NoErrorOK();
#endif  
}//fin init puertos++++++++++++++++++++++++


void init_Display(void){
// unsigned char init[]=0x00,0x00}; 
 //unsigned char delays={  1,  2};
    // display_data(&init[0],&delays[0])
 
}//fin de init_Display+++++++++++++++++++++