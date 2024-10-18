#include <stdio.h>
#include <wiringPi.h>
//LED PIN WIRINpi 0 is bcm_gpio 17.
#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7
#define WR   8
#define BUSY 9

int main(void){
  printf("\nInizializacion de Display");
  wiringPiSetup();

  pinMode(BIT0,OUTPUT);
  pinMode(BIT1,OUTPUT);
  pinMode(BIT2,OUTPUT);
  pinMode(BIT3,OUTPUT);
  pinMode(BIT4,OUTPUT);
  pinMode(BIT5,OUTPUT);
  pinMode(BIT6,OUTPUT);
  pinMode(WR,OUTPUT);
  pinMode(BUSY,OUTPUT);

for(;;){
   digitalWrite(BIT5,HIGH);
   delay(500);
   digitalWrite(BIT5,LOW);

}//fin for++++++++++++++++++++



}//fin de main+++++++++++++++++++++++++++++++++++++++

