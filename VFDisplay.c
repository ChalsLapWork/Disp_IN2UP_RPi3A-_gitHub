#include "system.h"
#include "queue.h"
#include "VFD.h"
#include "errorController.h"
#include <stdio.h>
#include "VFDmenu.h"
#include "BarraDisplay.h"

extern struct _DISPLAY_VFD_ vfd;


/*funcion que nos da las coordenadas de las cajas en  y las cajas con pen=1 con, caja BOX_VACIA hasta 
 *  la 8,  y la caja BOX_LLENA es de la box=9 hasta box=17; */ 
void getBoxPattern(unsigned char box,unsigned char *mode,unsigned char *x1,
                   unsigned char *y1,unsigned char *x2,unsigned char *y2){

 switch(vfd.menu.contexto.Actual){	
   case PORTAL_INICIO:
    	  switch(box){
			 case 1 : *x1=POS_X1_BOX3_PI;*y1=POS_Y1_BOX3_PI;*x2=POS_X2_BOX3_PI;*y2=POS_Y2_BOX3_PI;break;  
			 case 2 : *x1=POS_X1_BOX4_PI;*y1=POS_Y1_BOX4_PI;*x2=POS_X2_BOX4_PI;*y2=POS_Y2_BOX4_PI;break;
			 case 3 : *x1=POS_X1_BOX5_PI;*y1=POS_Y1_BOX5_PI;*x2=POS_X2_BOX5_PI;*y2=POS_Y2_BOX5_PI;break;
			 case 4 : *x1=POS_X1_BOX6_PI;*y1=POS_Y1_BOX6_PI;*x2=POS_X2_BOX6_PI;*y2=POS_Y2_BOX6_PI;break;
			 case 5 : *x1=POS_X1_BOX7_PI;*y1=POS_Y1_BOX7_PI;*x2=POS_X2_BOX7_PI;*y2=POS_Y2_BOX7_PI;break;
			 case 6 : *x1=POS_X1_BOX8_PI;*y1=POS_Y1_BOX8_PI;*x2=POS_X2_BOX8_PI;*y2=POS_Y2_BOX8_PI;break;
			 case 7 : *x1=POS_X1_BOX9_PI;*y1=POS_Y1_BOX9_PI;*x2=POS_X2_BOX9_PI;*y2=POS_Y2_BOX9_PI;break;
			 case 8 : *x1=POS_X1_BOX10_PI;*y1=POS_Y1_BOX10_PI;*x2=POS_X2_BOX10_PI;*y2=POS_Y2_BOX10_PI;break;
	   		 case 9 : *x1=POS_X1_BOX11_PI;*y1=POS_Y1_BOX11_PI;*x2=POS_X2_BOX11_PI;*y2=POS_Y2_BOX11_PI;break;
			 case 10: *x1=POS_X1_BOX12_PI;*y1=POS_Y1_BOX12_PI;*x2=POS_X2_BOX12_PI;*y2=POS_Y2_BOX12_PI;break;
			 case 11: *x1=POS_X1_BOX13_PI;*y1=POS_Y1_BOX13_PI;*x2=POS_X2_BOX13_PI;*y2=POS_Y2_BOX13_PI;break;
			 case 12: *x1=POS_X1_BOX14_PI;*y1=POS_Y1_BOX14_PI;*x2=POS_X2_BOX14_PI;*y2=POS_Y2_BOX14_PI;break;
			 case 13: *x1=POS_X1_BOX15_PI;*y1=POS_Y1_BOX15_PI;*x2=POS_X2_BOX15_PI;*y2=POS_Y2_BOX15_PI;break;
	         case 14: *x1=POS_X1_BOX16_PI;*y1=POS_Y1_BOX16_PI;*x2=POS_X2_BOX16_PI;*y2=POS_Y2_BOX16_PI;break;
			 case 15: *x1=POS_X1_BOX17_PI;*y1=POS_Y1_BOX17_PI;*x2=POS_X2_BOX17_PI;*y2=POS_Y2_BOX17_PI;break;
			 case 16: *x1=POS_X1_BOX18_PI;*y1=POS_Y1_BOX18_PI;*x2=POS_X2_BOX18_PI;*y2=POS_Y2_BOX18_PI;break;
			 case 17: *x1=POS_X1_BOX19_PI;*y1=POS_Y1_BOX19_PI;*x2=POS_X2_BOX19_PI;*y2=POS_Y2_BOX19_PI;break; 
			 default:*x1=POS_X1_BOX3_PI;*y1=POS_Y1_BOX3_PI;*x2=POS_X2_BOX3_PI;*y2=POS_Y2_BOX3_PI;
			         box=1;break;//iniciaiacion del box0
			 }//fin switch
	         if(box<9)
	        	 *mode=BOX_VACIA;
	         else *mode=BOX_LLENA;
	         break;// case PORTAL INICIO   Completado
    case AJUSTE_DE_VIBRACION:
        switch(box){
		 case 1 : *x1=POS_X1_BOX3_AV;*y1=POS_Y1_BOX3_AV;*x2=POS_X2_BOX3_AV;*y2=POS_Y2_BOX3_AV;break;  
		 case 2 : *x1=POS_X1_BOX4_AV;*y1=POS_Y1_BOX4_AV;*x2=POS_X2_BOX4_AV;*y2=POS_Y2_BOX4_AV;break;
		 case 3 : *x1=POS_X1_BOX5_AV;*y1=POS_Y1_BOX5_AV;*x2=POS_X2_BOX5_AV;*y2=POS_Y2_BOX5_AV;break;
		 case 4 : *x1=POS_X1_BOX6_AV;*y1=POS_Y1_BOX6_AV;*x2=POS_X2_BOX6_AV;*y2=POS_Y2_BOX6_AV;break;
		 case 5 : *x1=POS_X1_BOX7_AV;*y1=POS_Y1_BOX7_AV;*x2=POS_X2_BOX7_AV;*y2=POS_Y2_BOX7_AV;break;
		 case 6 : *x1=POS_X1_BOX8_AV;*y1=POS_Y1_BOX8_AV;*x2=POS_X2_BOX8_AV;*y2=POS_Y2_BOX8_AV;break;
		 case 7 : *x1=POS_X1_BOX9_AV;*y1=POS_Y1_BOX9_AV;*x2=POS_X2_BOX9_AV;*y2=POS_Y2_BOX9_AV;break;
		 case 8 : *x1=POS_X1_BOX10_AV;*y1=POS_Y1_BOX10_AV;*x2=POS_X2_BOX10_AV;*y2=POS_Y2_BOX10_AV;break;
   		 case 9 : *x1=POS_X1_BOX11_AV;*y1=POS_Y1_BOX11_AV;*x2=POS_X2_BOX11_AV;*y2=POS_Y2_BOX11_AV;break;
		 case 10: *x1=POS_X1_BOX12_AV;*y1=POS_Y1_BOX12_AV;*x2=POS_X2_BOX12_AV;*y2=POS_Y2_BOX12_AV;break;
		 case 11: *x1=POS_X1_BOX13_AV;*y1=POS_Y1_BOX13_AV;*x2=POS_X2_BOX13_AV;*y2=POS_Y2_BOX13_AV;break;
		 case 12: *x1=POS_X1_BOX14_AV;*y1=POS_Y1_BOX14_AV;*x2=POS_X2_BOX14_AV;*y2=POS_Y2_BOX14_AV;break;
		 case 13: *x1=POS_X1_BOX15_AV;*y1=POS_Y1_BOX15_AV;*x2=POS_X2_BOX15_AV;*y2=POS_Y2_BOX15_AV;break;
         case 14: *x1=POS_X1_BOX16_AV;*y1=POS_Y1_BOX16_AV;*x2=POS_X2_BOX16_AV;*y2=POS_Y2_BOX16_AV;break;
		 case 15: *x1=POS_X1_BOX17_AV;*y1=POS_Y1_BOX17_AV;*x2=POS_X2_BOX17_AV;*y2=POS_Y2_BOX17_AV;break;
		 case 16: *x1=POS_X1_BOX18_AV;*y1=POS_Y1_BOX18_AV;*x2=POS_X2_BOX18_AV;*y2=POS_Y2_BOX18_AV;break;
		 case 17: *x1=POS_X1_BOX19_AV;*y1=POS_Y1_BOX19_AV;*x2=POS_X2_BOX19_AV;*y2=POS_Y2_BOX19_AV;break; 
		 default:*x1=POS_X1_BOX3_AV;*y1=POS_Y1_BOX3_AV;*x2=POS_X2_BOX3_AV;*y2=POS_Y2_BOX3_AV;
		         box=1;break;//iniciaiacion del box0
		 }//fin switch
         if(box<9)
        	 *mode=BOX_VACIA;
         else *mode=BOX_LLENA;
       break;//  //fin AJUSTE DE VIBRACION
   case AJUSTE_PARAMETRICO_DE_PRODUCTO:
    		switch(box){
    				 case 1 : *x1=POS_X1_BOX3_AP;*y1=POS_Y1_BOX3_AP;*x2=POS_X2_BOX3_AP;*y2=POS_Y2_BOX3_AP;break;  
    				 case 2 : *x1=POS_X1_BOX4_AP;*y1=POS_Y1_BOX4_AP;*x2=POS_X2_BOX4_AP;*y2=POS_Y2_BOX4_AP;break;
    				 case 3 : *x1=POS_X1_BOX5_AP;*y1=POS_Y1_BOX5_AP;*x2=POS_X2_BOX5_AP;*y2=POS_Y2_BOX5_AP;break;
    				 case 4 : *x1=POS_X1_BOX6_AP;*y1=POS_Y1_BOX6_AP;*x2=POS_X2_BOX6_AP;*y2=POS_Y2_BOX6_AP;break;
    				 case 5 : *x1=POS_X1_BOX7_AP;*y1=POS_Y1_BOX7_AP;*x2=POS_X2_BOX7_AP;*y2=POS_Y2_BOX7_AP;break;
    				 case 6 : *x1=POS_X1_BOX8_AP;*y1=POS_Y1_BOX8_AP;*x2=POS_X2_BOX8_AP;*y2=POS_Y2_BOX8_AP;break;
    				 case 7 : *x1=POS_X1_BOX9_AP;*y1=POS_Y1_BOX9_AP;*x2=POS_X2_BOX9_AP;*y2=POS_Y2_BOX9_AP;break;
    				 case 8 : *x1=POS_X1_BOX10_AP;*y1=POS_Y1_BOX10_AP;*x2=POS_X2_BOX10_AP;*y2=POS_Y2_BOX10_AP;break;
    		   		 case 9 : *x1=POS_X1_BOX11_AP;*y1=POS_Y1_BOX11_AP;*x2=POS_X2_BOX11_AP;*y2=POS_Y2_BOX11_AP;break;
    				 case 10: *x1=POS_X1_BOX12_AP;*y1=POS_Y1_BOX12_AP;*x2=POS_X2_BOX12_AP;*y2=POS_Y2_BOX12_AP;break;
    				 case 11: *x1=POS_X1_BOX13_AP;*y1=POS_Y1_BOX13_AP;*x2=POS_X2_BOX13_AP;*y2=POS_Y2_BOX13_AP;break;
    				 case 12: *x1=POS_X1_BOX14_AP;*y1=POS_Y1_BOX14_AP;*x2=POS_X2_BOX14_AP;*y2=POS_Y2_BOX14_AP;break;
    				 case 13: *x1=POS_X1_BOX15_AP;*y1=POS_Y1_BOX15_AP;*x2=POS_X2_BOX15_AP;*y2=POS_Y2_BOX15_AP;break;
    		         case 14: *x1=POS_X1_BOX16_AP;*y1=POS_Y1_BOX16_AP;*x2=POS_X2_BOX16_AP;*y2=POS_Y2_BOX16_AP;break;
    				 case 15: *x1=POS_X1_BOX17_AP;*y1=POS_Y1_BOX17_AP;*x2=POS_X2_BOX17_AP;*y2=POS_Y2_BOX17_AP;break;
    				 case 16: *x1=POS_X1_BOX18_AP;*y1=POS_Y1_BOX18_AP;*x2=POS_X2_BOX18_AP;*y2=POS_Y2_BOX18_AP;break;
    				 case 17: *x1=POS_X1_BOX19_AP;*y1=POS_Y1_BOX19_AP;*x2=POS_X2_BOX19_AP;*y2=POS_Y2_BOX19_AP;break; 
    				 default:*x1=POS_X1_BOX3_AP;*y1=POS_Y1_BOX3_AP;*x2=POS_X2_BOX3_AP;*y2=POS_Y2_BOX3_AP;
    				         box=1;break;//iniciaiacion del box0
    				 }//fin switch
    		         if(box<9)
    		        	 *mode=BOX_VACIA;
    		         else *mode=BOX_LLENA;
    	   break;   //FIN ajuste PARAMETRICO DE PRODUCTO
   case PARAMETROS_DE_DETECCION:
      		switch(box){
      				 case 1 : *x1=POS_X1_BOX3_PD;*y1=POS_Y1_BOX3_PD;*x2=POS_X2_BOX3_PD;*y2=POS_Y2_BOX3_PD;break;  
       				 case 2 : *x1=POS_X1_BOX4_PD;*y1=POS_Y1_BOX4_PD;*x2=POS_X2_BOX4_PD;*y2=POS_Y2_BOX4_PD;break;
       				 case 3 : *x1=POS_X1_BOX5_PD;*y1=POS_Y1_BOX5_PD;*x2=POS_X2_BOX5_PD;*y2=POS_Y2_BOX5_PD;break;
       				 case 4 : *x1=POS_X1_BOX6_PD;*y1=POS_Y1_BOX6_PD;*x2=POS_X2_BOX6_PD;*y2=POS_Y2_BOX6_PD;break;
       				 case 5 : *x1=POS_X1_BOX7_PD;*y1=POS_Y1_BOX7_PD;*x2=POS_X2_BOX7_PD;*y2=POS_Y2_BOX7_PD;break;
       				 case 6 : *x1=POS_X1_BOX8_PD;*y1=POS_Y1_BOX8_PD;*x2=POS_X2_BOX8_PD;*y2=POS_Y2_BOX8_PD;break;
       				 case 7 : *x1=POS_X1_BOX9_PD;*y1=POS_Y1_BOX9_PD;*x2=POS_X2_BOX9_PD;*y2=POS_Y2_BOX9_PD;break;
       				 case 8 : *x1=POS_X1_BOX10_PD;*y1=POS_Y1_BOX10_PD;*x2=POS_X2_BOX10_PD;*y2=POS_Y2_BOX10_PD;break;
       		   		 case 9 : *x1=POS_X1_BOX11_PD;*y1=POS_Y1_BOX11_PD;*x2=POS_X2_BOX11_PD;*y2=POS_Y2_BOX11_PD;break;
       				 case 10: *x1=POS_X1_BOX12_PD;*y1=POS_Y1_BOX12_PD;*x2=POS_X2_BOX12_PD;*y2=POS_Y2_BOX12_PD;break;
       				 case 11: *x1=POS_X1_BOX13_PD;*y1=POS_Y1_BOX13_PD;*x2=POS_X2_BOX13_PD;*y2=POS_Y2_BOX13_PD;break;
       				 case 12: *x1=POS_X1_BOX14_PD;*y1=POS_Y1_BOX14_PD;*x2=POS_X2_BOX14_PD;*y2=POS_Y2_BOX14_PD;break;
       				 case 13: *x1=POS_X1_BOX15_PD;*y1=POS_Y1_BOX15_PD;*x2=POS_X2_BOX15_PD;*y2=POS_Y2_BOX15_PD;break;
       		         case 14: *x1=POS_X1_BOX16_PD;*y1=POS_Y1_BOX16_PD;*x2=POS_X2_BOX16_PD;*y2=POS_Y2_BOX16_PD;break;
       				 case 15: *x1=POS_X1_BOX17_PD;*y1=POS_Y1_BOX17_PD;*x2=POS_X2_BOX17_PD;*y2=POS_Y2_BOX17_PD;break;
       				 case 16: *x1=POS_X1_BOX18_PD;*y1=POS_Y1_BOX18_PD;*x2=POS_X2_BOX18_PD;*y2=POS_Y2_BOX18_PD;break;
       				 case 17: *x1=POS_X1_BOX19_PD;*y1=POS_Y1_BOX19_PD;*x2=POS_X2_BOX19_PD;*y2=POS_Y2_BOX19_PD;break; 
       				 default:*x1=POS_X1_BOX3_PD;*y1=POS_Y1_BOX3_PD;*x2=POS_X2_BOX3_PD;*y2=POS_Y2_BOX3_PD;
       				         box=1;break;//iniciaiacion del box0
       				 }//fin switch
       		         if(box<9)
       		        	 *mode=BOX_VACIA;
       		         else *mode=BOX_LLENA;
        	   break;   //FIN ajuste PARAMETRICO DE PRODUCTO
      default:break;
      }//fin PRIMER SWITCH           	         		    		     	  
}//fin de get patter of the boxes-------------------------------------------------------------------------------
