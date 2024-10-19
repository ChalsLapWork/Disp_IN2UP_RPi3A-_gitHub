#include <wiringPi.h>
#include "system.h"
#include "init.h"


void configPuertos(void){
pinMode(BIT0,OUTPUT);
  pinMode(BIT1,OUTPUT);
  pinMode(BIT2,OUTPUT);
  pinMode(BIT3,OUTPUT);
  pinMode(BIT4,OUTPUT);
  pinMode(BIT5,OUTPUT);
  pinMode(BIT6,OUTPUT);
  pinMode(WR,OUTPUT);
  pinMode(BUSY,OUTPUT);
}//fin init puertos++++++++++++++++++++++++


void init_Display(void){
 unsigned char init=  {0x00,0x00}; 
 unsigned char delays={  1,  2};
     display_data(&init[0],&delays[0])
 
}//fin de init_Display+++++++++++++++++++++