#include <stdio.h>
#include <wiringPi.h>
#include "system.h"
#include "init.h"
#include "queue.h"
#include "VFDisplay.h"


int main(void){
  printf("\nInizializacion de Display");
  wiringPiSetup();
  configPuertos();
  init_queues();
  Init_VFD();

for(;;){
   digitalWrite(BIT5_PIN,HIGH);
   delay(500);
   digitalWrite(BIT5_PIN,LOW);

}//fin for++++++++++++++++++++



}//fin de main+++++++++++++++++++++++++++++++++++++++

