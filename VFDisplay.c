#inclue "system.h"
#include "queue.h"
#include "VFD.h"

extern struct _VFD_ vfd;

//metodo unico de padre unico  y sin instancias
void Init_VFD( void){
   
  //if(recurso.solicitar(_VFD_,PID_xInit_VFD1)){
  VFDcommand(0x1BU); // init display  ESC@= 1BH,40H
  VFDcommand(0x40U);	 // Setting return to default values 
  FontSizeVFD(FONTSIZE2);//TAMAÑO DE fuente 2->8x16
  vfd.config.bits.init_VFD=TRUE;

			  
}//fin init VFD -------------------------------------------------------------------
