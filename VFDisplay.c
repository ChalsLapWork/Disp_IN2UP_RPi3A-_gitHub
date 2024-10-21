#include "system.h"
#include "queue.h"
#include "VFD.h"


extern struct _DISPLAY_VFD_ vfd;

//metodo unico de padre unico  y sin instancias
void Init_VFD( void){
unsigned char ret=0;   
  //if(recurso.solicitar(_VFD_,PID_xInit_VFD1)){
  ret+=VFDcommand(0x1BU); // init display  ESC@= 1BH,40H
  ret+=VFDcommand(0x40U);	 // Setting return to default values 
  ret+=FontSizeVFD(FONTSIZE2);//TAMAÑO DE fuente 2->8x16
  if(ret==3)
       vfd.config.bits.init_VFD=TRUE;
  else errorCritico("\nNo se inizializa el display");	   	  
}//fin init VFD -------------------------------------------------------------------
