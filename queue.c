
#include "queue.h"


struct _DISPLAY_VFD_ vfd;

void init_queue(void){
    vfd.config.byte1=0;//init all parameter into zero


}//fin init queue++++++++++


/*parametro 
 * 1: La fifo a inizializar
 * 2: pointer to first element array of fifo
 * 3:pointer to last element array of fifo
 * 4: size of fifo 
 * version 300322-1205
 * version 310322-1637 add reset as general */
void init_FIFO_General_1byte(struct _FIFO_1byte_ *s,
     unsigned char *h,unsigned char size){//FIFO_SER_KEYPAD[SIZE_SER_KEYPAD];
	s->head=h;
	s->tail=h+size-1;
	s->pop=s->tail;
	s->push=s->tail;
	s->ncount=0;
	s->popf=FIFO_general_1byte_pop;
	s->appendByte=FIFO_general_1byte_push;
	s->size=size;
	s->resetFIFO=reset_FIFO_general_UChar;
}//fin init_FIFO_RX_serial_Keypad------------------------------------------


//return FALSE if is empty
/* version 300322-1156*/
unsigned char FIFO_general_1byte_pop(unsigned char *dato,struct _FIFO_1byte_ *s){	
	if(s->ncount==0)
		return FALSE;
	if(s->ncount==1){
		*dato=*(s->pop);//solo hay un dato en la FIFO
		*(s->pop)=0;//vaciamos nodo
		s->pop=s->push=s->tail;//reajustamos todo de inicio
		s->ncount=0;}
	else{*dato=*(s->pop);//solo hay un dato en la FIFO
	     *(s->pop)=0;//vaciamos nodo
	     if(s->ncount>0)
	    	  s->ncount--;
		 if(s->pop==s->head)
			    s->pop=s->tail;
		 else s->pop--;}
return TRUE;
}//FIFO_general_1byte_push------------------------------------------

//regresa TRUE  si TODO bien  vfd.f1.append(14,0,_BOX_);
/* El buffer de la FIFO se satura son mas los que entran que los que salen
 * se satura, vamos a poner un seguro para que vacie la FIFO cuando se sature
 * y los datos que llegan cuando este saturado que se pierdan
 * RegrESA  FALSE si esta llena
 *   version 39.22.5.0
 * */
unsigned char FIFO_general_1byte_push(unsigned char dato,struct _FIFO_1byte_ *s){
auto unsigned char ret=0;
	  if(!(s->size>s->ncount)) 
		   return FALSE;//FIFO llena
	  if(s->ncount==0){
		   s->pop=s->push=s->tail;//emparejamos pointers
		   *(s->push)=dato;
		   s->push--;
	       s->ncount++;ret=TRUE;}
	  else{if(s->push==s->head){
		      if(s->tail==s->pop){
		    	  *(s->push)=dato;
		    	  s->push=s->pop;//esta llena
		    	  s->ncount++;
		    	  if(s->ncount==s->size){//si cupo uno
		    		    ret=TRUE;}//SE inserto dato en ultimo lugar vacio
		    	  else{__asm(nop);__asm(Halt);}}//error de software
		      else{*(s->push)=dato;s->push=s->tail;
		           s->ncount++;ret=TRUE;}}
	       else{if(s->push-1==s->pop){//nos recorreremos para atras y no topamos con pop
		           *(s->push)=dato;
		           s->push=s->pop;
		           s->ncount++;
		           if(s->ncount==s->size){//se acaba de llenar fifo pero si cupo uno
  		    		    ret=TRUE;}//SE inserto dato en ultimo lugar vacio
   		    	   else{__asm(nop);__asm(Halt);}}//error de software
	            else{*(s->push)=dato;s->push--;
	                 s->ncount++;ret=TRUE;}}}
return ret;
}//FIFO_general_1byte_push------------------------------------------



//se resetea toda la fifo y todo queda cmo de inicio
//  serial.resetFIFO=reset_FIFO_serial_TX;
// version 310322-1626 
void reset_FIFO_general_UChar(struct _FIFO_1byte_ *s,unsigned char *arr,unsigned short int  size){
	   s->pop=s->push=s->tail;
	   s->ncount=0;
	   cleanArray(arr,size,0);
	  
}//fin reset_FIFO_serial_TX---fin se resetea toda la fifo

