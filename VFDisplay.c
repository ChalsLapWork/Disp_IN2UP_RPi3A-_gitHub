#include "queue.h"
#include "VFD.h"

extern struct _VFD_ vfd;

//metodo unico de padre unico  y sin instancias
void Init_VFD( void){
   if(vfd.config.bits.init_VFD)
  if(recurso.solicitar(_VFD_,PID_xInit_VFD1)){


  VFDcommand(0x1BU); // init display  ESC@= 1BH,40H


				  (*estado)++;
			  break;
	  case 4: if(VFDcommand(0x40U))	 // Setting return to default values
                  (*estado)++;
			  break;
	  case 5:if(FontSizeVFD(FONTSIZE2,mem2))//TAMAÑO DE fuente 2->8x16
		           (*estado)++;
			  break;
	  case 6:
			  vfd.bits.b.VDF_busy=FALSE;//estamos usando el VFD
			  *estado=0;
			  vfd.bits.b.finit_VFD=TRUE;//se init el VFd
			  menu.ret.u.bit.fInit_VFD=TRUE;
			  recurso.devolver(_VFD_,PID_xInit_VFD1);
			  SO.boot.bits.b.init_Disp=TRUE;//SE inizializo el display
			  break;
	  default: *estado=1;break;}}
//keyboard_IRQ=TRUE;
return 0;		
}//fin init VFD -------------------------------------------------------------------
