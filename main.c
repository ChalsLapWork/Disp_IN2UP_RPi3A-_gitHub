#include <stdio.h>
#include <wiringPi.h>
#include "system.h"
#include "init.h"
#include "queue.h"
#include "VFDisplay.h"
#include <signal.h>
#include <unistd.h>
#include "errorController.h"
#include <pthread.h>


void signal_handler(int signalnum){
    #if(debug_level1==1)
       printf(" signal:%d",signalnum);
    #endif
    Terminar_subProcesos();
}//fin manejador de signal

int main(void){
  printf("\nInsight v3");
  wiringPiSetup();  
  signal(SIGINT,signal_handler);//asocia el manejador de salida del programa
  configPuertos();
  init_queues();
  
  printf("\n       Hilo Principal Terminado");
  NoErrorOK();
  printf("\n ");
  pthread_exit(NULL);//exit(1);

  //sleep(500);//tiempo para transmitir los printf anteriores antes de matar el proceso
   
  //pthread_exit(NULL);//espera alos hijos en la memoria para  que los hilos hijos terminen para que vivan hasta sus ultimas lineas de codigo
//for(;;){
   //digitalWrite(BIT5_PIN,HIGH);
   //delay(500);
   //digitalWrite(BIT5_PIN,LOW);}//fin for++++++++++++++++++++



}//fin de main+++++++++++++++++++++++++++++++++++++++

