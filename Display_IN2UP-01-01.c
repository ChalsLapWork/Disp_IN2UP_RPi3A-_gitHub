#include <stdio.h>
#include <wiringPi.h>
#include "init.h"


int main(void){
  printf("\nInizializacion de Display");
  wiringPiSetup();
  configPuertos();
  

for(;;){
   digitalWrite(BIT5,HIGH);
   delay(500);
   digitalWrite(BIT5,LOW);

}//fin for++++++++++++++++++++



}//fin de main+++++++++++++++++++++++++++++++++++++++

