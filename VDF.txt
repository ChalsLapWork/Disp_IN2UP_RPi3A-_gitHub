#include <pthread>
#include "VFD.h"
#include <stdio.h>

typedef struct{
    pthread_t hilo_id;//id del hilo
    int isAlive;//esta vivo?
}HiloInfo;



// pusimos estos delay y el tamaño de la Font no obedecia
	//y se ponia chica
//con estos delay ya obedece pero no sabemos si todos o cual
// instancia s1,s2
//METODO  mono-Padre Con-Instancia No almismo Tiempo
unsigned char FontSizeVFD(unsigned char m){
auto unsigned char estado;
auto unsigned char ret=0;
auto unsigned char inst[2];
auto unsigned short int n=10000; 

    VFDcommand_init(0x1FU,p+3);// font size select of a character command
    delay_us_VFD(n);//<--pusimos estos delay
    VFDcommand_init(0x28U);//  comando 1fh,28h,67h,01h,m
    delay_us_VFD(n);//<--pusimos estos delay
    VFDcommand_init(0x67U);//m= 01h  6x8 font,   m=02h->8x16 font
    delay_us_VFD(n);//<--pusimos estos delay
    VFDcommand_init(0x01U);//m=03h-> 12x24   m=04h->16x32 font
    delay_us_VFD(n);//<--pusimos estos delay
    VFDcommand_init(m);
		        
return ret;
}// fin font size for the VFD----------------------------------------------------



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

//oct-24
//REGRESA TRUE si ya se ejecuto todo el comando hasta el final
unsigned char VFDcommand_init(unsigned char cmd){
unsigned char ret;
//static unsigned char inst[2];//variables que usan los procesos hijos multipadre
	//TI1_delays_Init();
	//delay1us();
	//con 1 ms se le quito el error de FontSize
	//con 100us tiene error de FontSize
	//con 500us se le quito el error de FontSize
	//con 250us se le quito el error de FontSize
	//Se le quito el error con 125us lo dejamos en 125us
 
      ret+=delay_us_VFD(225);
      ret+=VFDserial_SendChar1(cmd);
      ret+=delay_us_VFD(225);
      if(ret==3) ret=TRUE;else ret=FALSE;
return ret;	
}//fin vfd command----------------------------------------------------

//delay con sistema operativo------------------------------------
//regresa true cuando se cumpla todo el methodo hasta el final
unsigned char delay_us_VFD(unsigned short int t){
auto unsigned char ret=0;
union W7{//access word: 
	unsigned  short int wordx;   //   	0xaabb        //         aa
	unsigned char byte[2];        //byte[0]=aa,byte[1]=bb
}w16; 

  w16.wordx=t;
  if(vfd.f1.append(w16.byte[0],w16.byte[1],_DELAY_US))
        ret=TRUE;
return ret;    
}//--------------------------------------------------
