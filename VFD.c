#include "VFD.h"
#include "system.h"
#include "queue.h"

extern struct _DISPLAY_VFD_ vfd;




//VFD comando en MODO operativo----------------------------------
//REGRESA TRUE si ya se ejecuto todo el comando hasta el final
//instancia 1 y dos 
// funcion mono-Padre || Solo puede tener un padre a la vez
unsigned char VFDcommand(unsigned char cmd){ //unsigned char *p){
	//con 1 ms se le quito el error de FontSize
	//con 100us tiene error de FontSize
	//con 500us se le quito el error de FontSize
	//con 250us se le quito el error de FontSize
	//Se le quito el error con 125us lo dejamos en 125us
return VFDserial_SendChar1(cmd);	    	   
}//fin vfd command----------------------------------------------------


// pusimos estos delay y el tamaño de la Font no obedecia
	//y se ponia chica
//con estos delay ya obedece pero no sabemos si todos o cual
// instancia s1,s2
//METODO  mono-Padre Con-Instancia No almismo Tiempo
unsigned char FontSizeVFD(unsigned char m){
auto unsigned short int n=10000;
auto unsigned char ret=0;

    ret+=VFDcommand_init(0x1FU);// font size select of a character command
	ret+=delay_us_VFD(n);//<--pusimos estos delay
	ret+=VFDcommand_init(0x28U);//  comando 1fh,28h,67h,01h,m
    ret+=delay_us_VFD(n);//<--pusimos estos delay
	ret+=VFDcommand_init(0x67U);//m= 01h  6x8 font,   m=02h->8x16 font
	ret+=delay_us_VFD(n);//<--pusimos estos delay
	ret+=VFDcommand_init(0x01U);//m=03h-> 12x24   m=04h->16x32 font
	ret+=delay_us_VFD(n);//<--pusimos estos delay
	ret+=VFDcommand_init(m);
	if(ret==9) ret=TRUE;else ret=FALSE;
return ret;
}// fin font size for the VFD----------------------------------------------------


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


//void VFDserial_SendChar1(unsigned char c){
//	vfd.f1.append(c,0,_CHAR_);// FIFO_Display_DDS_Char_push(c,0xFE);//0xFE means that is just a char display          
//}//fin VFDserial_SendChar_DDS---------------------------------
unsigned char VFDserial_SendChar1(unsigned char c){
	return vfd.f1.append(c,0,_CHAR_);
}//------------------------------------------------------------------



//REGRESA TRUE si ya se ejecuto todo el comando hasta el final
unsigned char VFDcommand_init(unsigned char cmd){
auto unsigned char ret=FALSE;
	//delay1us();
	//con 1 ms se le quito el error de FontSize
	//con 100us tiene error de FontSize
	//con 500us se le quito el error de FontSize
	//con 250us se le quito el error de FontSize
	//Se le quito el error con 125us lo dejamos en 125us
 
  
    ret+=delay_us_VFD(225);
    ret+=VFDserial_SendChar1(cmd);
    ret+=delay_us_VFD(225);
    if(ret==3)ret=TRUE;else ret=FALSE;		    
return ret;	
}//fin vfd command----------------------------------------------------
