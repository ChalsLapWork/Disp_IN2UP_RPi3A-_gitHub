#include <pthread>
#include "VFD.h"
#include <stdio.h>

typedef struct{
    pthread_t hilo_id;//id del hilo
    int isAlive;//esta vivo?
}HiloInfo;


//VFD comando en MODO operativo----------------------------------
//REGRESA TRUE si ya se ejecuto todo el comando hasta el final
//instancia 1 y dos 
// funcion mono-Padre || Solo puede tener un padre a la vez
void VFDcommand(unsigned char cmd){ //unsigned char *p){
//auto unsigned char estado11;
//auto unsigned char ret=FALSE;
//static unsigned char Semaforo;//SI el semaforo esta en cero se puede usar o si el semaforo vale lo que el padre
//static unsigned char inst[2];//variables que usan los procesos hijos multipadre
	//TI1_delays_Init();
	//delay1us();
	//con 1 ms se le quito el error de FontSize
	//con 100us tiene error de FontSize
	//con 500us se le quito el error de FontSize
	//con 250us se le quito el error de FontSize
	//Se le quito el error con 125us lo dejamos en 125us

    // return VFDserial_SendChar1(cmd);///compatible con el programa antiguo
    if(!VFDserial_SendChar1(cmd)){///compatible con el programa antiguo
            printf("\nerror de sendChar1");
            exit(1);}
}//fin vfd comando


//void VFDserial_SendChar1(unsigned char c){
//	vfd.f1.append(c,0,_CHAR_);// FIFO_Display_DDS_Char_push(c,0xFE);//0xFE means that is just a char display          
//}//fin VFDserial_SendChar_DDS---------------------------------
unsigned char VFDserial_SendChar1(unsigned char c){
	return vfd.f1.append(c,0,_CHAR_);
}//------------------------------------------------------------------

