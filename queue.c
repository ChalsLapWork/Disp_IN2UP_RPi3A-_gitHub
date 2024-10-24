#include "system.h"
#include "queue.h"
#include "string.h"
#include "errorController.h"
#if(debug_level1==1)
  #include <stdio.h>
#endif
#include <pthread.h>

typedef struct{
  unsigned char data[SIZE_MAX_FIFO];
  unsigned char head,tail;
  pthread_mutex_t  lock;//mutex lock
  pthread_cond_t   cond;//mutex condicion
}FIFO_VFD;

struct _DISPLAY_VFD_ vfd;
FIFO_VFD vfdtx;//fifo de transmision vfd 
void init_Queue_with_Thread(FIFO_VFD *q);
int dequeue(FIFO_VFD *q);
void enqueue(FIFO_VFD *q);
unsigned char is_full_Queue(FIFO_VFD *q);
unsigned char is_empty_Queue(FIFO_VFD *q);
void* SubProceso_Tx_VFD(void* arg);
pthread_t Proc_Tx_VFD;//Proceso Transmisor al VFD, para despliegue de pantalla

unsigned char  buffer6[SIZE_BUFFER6];//FIFO graficos con S.O, aqui guarda el dato
unsigned char  buffer7[SIZE_BUFFER6];//FIFO graficos con SO. aqui guarda el parametro=char|box|pos|
unsigned char  buffer8[SIZE_BUFFER6];//FIFO graficos con SO. aqui guarda el parametro numero 3


void init_queues(void){
	init_FIFO_General_1byte(&vfd.x,&buffer6[0],SIZE_BUFFER6);
    init_FIFO_General_1byte(&vfd.y,&buffer7[0],SIZE_BUFFER6);
    init_FIFO_General_1byte(&vfd.p,&buffer8[0],SIZE_BUFFER6);
     	  
    vfd.config.bytes1=0;//init all parameter into zero
    vfd.f1.append= vfd_FIFO_push;
	vfd.f1.pop=vfd_FIFO_pop;                                                                                                                                                                                                                                                                                                                                                                                                                      
	vfd.f1.resetFIFOS=vfd_FIFOs_RESET;
	init_Queue_with_Thread(&vfdtx);//fifos Transmisor data al Display
	if(!pthread_create(&Proc_Tx_VFD,NULL,SubProceso_Tx_VFD,&vfdtx))//ret==0 :all OK
	    vfd.config.bits.Proc_VFD_Tx_running=TRUE;//esta corriendo el proceso de TX al VFD
#if (debug_level1==1) 
  printf("\nQueues Inizializadas");
#endif  

}//fin init queue++++++++++


void init_Queue_with_Thread(FIFO_VFD *q){
      q->head=q->tail=0;
	  pthread_mutex_init(&q->lock,NULL);//
	  pthread_cond_init(&q->cond,NULL);
}//fin de init FIFO transmit VFD+++++++++++++++++++++++++
  
unsigned char is_full_Queue(FIFO_VFD *q){
    return (q->tail+1)% SIZE_MAX_FIFO == q->head;
}//FIN DE  is full FIFO tx VFD +++++++++++++++++++++++++

unsigned char is_empty_Queue(FIFO_VFD *q){
 return q->tail==q->head;
}//fin de esta vacia la queue de transmision de VFD ++++++++

void enqueue(FIFO_VFD *q){
  pthread_mutex_lock(&q->lock);
    while (is_full_Queue(q)) {
        pthread_cond_wait(&q->cond, &q->lock);
    }
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->lock);
}//fin enqueue++++++++++++++++++++++++++++++++++++

int dequeue(FIFO_VFD *q) {
    pthread_mutex_lock(&q->lock);
    while (is_empty(q)) {
        pthread_cond_wait(&q->cond, &q->lock);
    }
    int value = q->data[q->front];
    q->head = (q->head + 1) % MAX_QUEUE_SIZE;
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->lock);
    return value;
}//fin de queue+++++++++++++++++++++++++++++++++

/*  Control de Display de VFD de despliegue
   por thread  */
void* SubProceso_Tx_VFD(void* arg) {
    FIFO_VFD* q = (FIFO_VFD*)arg;
    while (TRUE) {
        int data = dequeue(q);
        printf("Processed: %d\n", data);
    }
    return NULL;
}


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
	s->ncount=s->nOcupados=0;s->nLibres=size;
	s->popf=FIFO_general_1byte_pop;
	s->appendByte=FIFO_general_1byte_push;
	s->size=size;
	s->resetFIFO=reset_FIFO_general_UChar;
}//fin init_FIFO_RX_serial_Keypad------------------------------------------


//return FALSE if is empty
/* version 300322-1156*/
unsigned char FIFO_general_1byte_pop(unsigned char *dato,
                   struct _FIFO_1byte_ *s){	
	if(s->ncount==0){
        #if(debug_level1==1)
		    printf("\nFIFO LLENA");
	    #endif
		return FALSE;}
	if(s->ncount==1){
		*dato=*(s->pop);//solo hay un dato en la FIFO
		*(s->pop)=0;//vaciamos nodo
		s->pop=s->push=s->tail;//reajustamos todo de inicio
		s->ncount=0;s->nLibres++;s->nOcupados=0;}
	else{*dato=*(s->pop);
	     *(s->pop)=0;//vaciamos nodo
	     if(s->ncount>0){
	    	  s->ncount--;s->nLibres++;s->nOcupados++;}
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
unsigned char FIFO_general_1byte_push(unsigned char dato,
                                  struct _FIFO_1byte_ *s){
auto unsigned char ret=0;
	  if(s->nLibres==0) 
		   return FALSE;//FIFO llena
	  if(s->ncount==0){
		   s->pop=s->push=s->tail;//emparejamos pointers
		   *(s->push)=dato;
		   s->push--;s->ncount++;s->nLibres--;s->nOcupados++;
           ret=TRUE;}
	  else{if(s->push==s->head){
		      if(s->tail==s->pop){
		    	  *(s->push)=dato;
		    	  s->push=s->pop;//esta llena
		    	  s->ncount++;s->nLibres--;s->nOcupados++;}
		      else{*(s->push)=dato;s->push=s->tail;
		           s->ncount++;s->nLibres--;s->nOcupados++;
                   ret=TRUE;}}
	       else{if(s->push-1==s->pop){//nos recorreremos para atras y no topamos con pop
		           *(s->push)=dato;
		           s->push=s->pop;
		           s->ncount++;s->nLibres--;s->nOcupados++;
                   if(s->nLibres>0){errorCritico("error de algoritmo de fifo");}
		           ret=TRUE;}
	             else{*(s->push)=dato;s->push--;
                          s->nLibres--;s->nOcupados++;
	                    s->ncount++;ret=TRUE;}}}
return ret;
}//FIFO_general_1byte_push---------------------------------------------



//se resetea toda la fifo y todo queda cmo de inici
// version 21-oct-24:10:15am
void reset_FIFO_general_UChar(struct _FIFO_1byte_ *s,
       unsigned char *arr,unsigned char  size){
	   s->pop=s->push=s->tail;
	   s->ncount=0;
	   cleanArray(arr,size,0);
	  
}//fin reset_FIFO_serial_TX---fin se resetea toda la fifo


//FIFO para ingresar un dato a desplegar vfd.f1.append(14,0,_BOX_);
//Return false|true   TRUE: si se agrego sin problemas
unsigned char vfd_FIFO_push(unsigned char x,unsigned char y,unsigned char p){
const unsigned char BYTES_BOX=250; //numero de ciclos, mas que bytes por comando de una box cdraw 
volatile unsigned char n=0;	
//static unsigned char control;
auto unsigned char ret=0;
    
    if(!(vfd.x.ncount<SIZE_BUFFER6))
    	 return FALSE;//esta muy llena la FIFO, espera un poco
    switch(p){//1100 0000 los dos MSB indican que proqrametro es
    	case _BOX_:if(x==0)
    		            return FALSE; 
    	           if(vfd.box.timer==0){
    	        	    vfd.box.timer=DELAY_TIME*BYTES_BOX;
    	        	    cleanArray(&vfd.box.boxs[0],SIZE_BOXES,0);
    	                return TRUE;}
    		       if(vfd.box.boxs[x]==0)
    		    	   vfd.box.boxs[x]++;        
    		       else{if(vfd.box.boxs[x]<250){
    		    	          vfd.box.boxs[x]++;
    		                  return TRUE;}
    		              else return TRUE;}
    	           break;              
    	case _CHAR_ :y='c';break;
    	case _PUNTO_:if((x==0)&&(y==0)){return(TRUE);}
    	             break;
    	case _RAYA_ : 
    	case _POS_  :break;
    	case _BOLD_:break;
    	default:break;}
     n=vfd.x.appendByte(x,&vfd.x);
	 n+=vfd.y.appendByte(y,&vfd.y);
	 n+=vfd.p.appendByte(p,&vfd.p);
	 if(n==3){//fifo llena
	      ret=TRUE;}
return ret;
}//fin vfd_FIFO_push-------------------------------------------


/*   */
unsigned char vfd_FIFO_pop(unsigned char *x,unsigned char *y,unsigned char *p){
unsigned char  r;	 
	   if(vfd.x.ncount==0){
		   if((vfd.y.ncount!=0)&&(vfd.p.ncount!=0)){
			       //__asm(Halt);//Debug error de software
		           errorCritico("\n error de Software de FIFO pop");}
	       return 0;}//FIFO vacia
	   else r=1;//FIFO regresa un valor
       vfd.x.popf(x,&vfd.x);
       vfd.y.popf(y,&vfd.y);
       vfd.p.popf(p,&vfd.p);
return r;	   
}//fin vfd_FIFO_pop------------------------------------------------------------

/* para el cambio de contexto todas los registros y FIFOs e
 * resetean*/
unsigned char vfd_FIFOs_RESET(void){
	vfd.config.bits.FIFOonReset=1;//se activa el reset, indica que estan en reseteo
	vfd.x.resetFIFO(&vfd.x,&buffer6[0],SIZE_BUFFER6);
	vfd.y.resetFIFO(&vfd.y,&buffer7[0],SIZE_BUFFER6);
	vfd.p.resetFIFO(&vfd.p,&buffer8[0],SIZE_BUFFER6);
 return TRUE;	
}//fin --------------------------------------------------------
