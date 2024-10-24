#include <stdio.h>
#include <wiringPi.h>
#include "system.h"
#include "init.h"
#include "queue.h"
#include "VFDisplay.h"
#include <signal.h>
#include <unistd.h>


void signal_handler(int signalnum){
    #if(debug_level1==1)
       printf(" signal:%d",signalnum);
    #endif
    Terminar_subProcesos();
}//fin manejador de signal

int main(void){
  printf("\nInizializacion de Display");
  wiringPiSetup();
  configPuertos();
  init_queues();
  Init_VFD();
  signal(SIGINT,signal_handler);//asocia el manejador de salida del programa
for(;;){
   digitalWrite(BIT5_PIN,HIGH);
   delay(500);
   digitalWrite(BIT5_PIN,LOW);

}//fin for++++++++++++++++++++



}//fin de main+++++++++++++++++++++++++++++++++++++++

