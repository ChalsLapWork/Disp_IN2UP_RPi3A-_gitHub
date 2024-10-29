#include "system.h"
#include "queue.h"
#include "string.h"
#include "errorController.h"
#include "VFD.h"
#if(debug_level1==1)
  #include <stdio.h>
#endif
#include <pthread.h>
#include <errno.h>

typedef struct{
  unsigned char Xdata[SIZE_MAX_FIFO];
  unsigned char Ydata[SIZE_MAX_FIFO];
  unsigned char Pdata[SIZE_MAX_FIFO];
  unsigned char head,tail;
  pthread_mutex_t  lock;//mutex lock
  pthread_cond_t   cond;//mutex condicion
  #if(SIZE_MAX_FIFO<255)
    unsigned char nLibres;
	unsigned char nOcupados;
  #endif
  struct _DISPLAY_VFD_ *v;//pointer to the control general of vfd
}FIFO_VFD;


struct _DISPLAY_VFD_ vfd;
FIFO_VFD vfdtx;//fifo de transmision vfd 
void init_Queue_with_Thread(FIFO_VFD *q);
unsigned char dequeue(FIFO_VFD *q,struct VFD_DATA *v );
unsigned char enqueue(FIFO_VFD *q,unsigned char X,unsigned char y,unsigned char p);
unsigned char is_full_Queue(FIFO_VFD *q);
unsigned char is_empty_Queue(FIFO_VFD *q);
void* SubProceso_Tx_VFD(void* arg);
pthread_t Proc_Tx_VFD;//Proceso Transmisor al VFD, para despliegue de pantalla
pthread_t Proc_Init_VFD;//Proceso para inizializar el VFD
pthread_cond_t cond_init_TX_VFD;//condicion de init VFD transmisor
pthread_mutex_t mutex_init_VFD;//mutex para init VFD y transmisor

unsigned char  buffer6[SIZE_BUFFER6];//FIFO graficos con S.O, aqui guarda el dato
unsigned char  buffer7[SIZE_BUFFER6];//FIFO graficos con SO. aqui guarda el parametro=char|box|pos|
unsigned char  buffer8[SIZE_BUFFER6];//FIFO graficos con SO. aqui guarda el parametro numero 3


void init_queues(void){
	init_FIFO_General_1byte(&vfd.x,&buffer6[0],SIZE_BUFFER6);
    init_FIFO_General_1byte(&vfd.y,&buffer7[0],SIZE_BUFFER6);
    init_FIFO_General_1byte(&vfd.p,&buffer8[0],SIZE_BUFFER6);
    printf("\n       Iniciando queueus");	  
    vfd.config.bytes1=0;//init all parameter into zero
    vfd.f1.append=vfd_FIFO_push;
	vfd.f1.pop=vfd_FIFO_pop;                                                                                                                                                                                                                                                                                                                                                                                                                      
	vfd.f1.resetFIFOS=vfd_FIFOs_RESET;
	vfdtx.v=&vfd;//misma estructura en los dos lados,
	init_Queue_with_Thread(&vfdtx);//fifos Transmisor data al Display
	vfd.config.bits.recurso_VFD_Ocupado=TRUE;//recurso ocupado, VFD nadie lo puede usar
	printf("\n       Creando Proceso Init VFD");
	switch(pthread_create(&Proc_Init_VFD,NULL,Init_VFD,&vfd)){
		case 0:NoErrorOK();break;
		case EAGAIN:errorCritico("Recursos insuficientes,Error de hilo init VFD");break;
		case EINVAL:errorCritico("Arg invalidos,Error de hilo init VFD");break;
		case EPERM:errorCritico("Permisos Insuficientes,Error de hilo init VFD");break;
		default:errorCritico("Error desconocido de hilo init VFD");break;}
	pthread_detach(Proc_Init_VFD);//que muera sin monitor y libere recursos
#if (debug_level1==1) 
   NoErrorOK();
#endif  

}//fin init queue++++++++++




void init_Queue_with_Thread(FIFO_VFD *q){
      q->head=q->tail=0;
	  q->nLibres=SIZE_MAX_FIFO-1;
	  q->nOcupados=0;
	  pthread_mutex_init(&q->lock,NULL);//
	  pthread_cond_init(&q->cond,NULL);
}//fin de init FIFO transmit VFD+++++++++++++++++++++++++
  
//true:is_Full. False: No_Full  
unsigned char is_full_Queue(FIFO_VFD *q){
   // return (q->tail+1)% SIZE_MAX_FIFO == q->head;
   if(q->nLibres==0)
       return TRUE;//IS FULL
   else return FALSE;//0: no esta lleno
}//FIN DE  is full FIFO tx VFD +++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++
unsigned char is_empty_Queue(FIFO_VFD *q){
 //return q->tail==q->head;
  if(q->nOcupados==0)
     return TRUE;//1:vacio fifo
  else return FALSE;//0:no esta vacio
}//fin de esta vacia la queue de transmision de VFD ++++++++

//encola regresa TRUE: si esta llena , FALSE: si esta vacia
unsigned char enqueue(FIFO_VFD *q,unsigned char x,unsigned char y,unsigned char p){
    if(is_full_Queue(q))
            return FALSE;//FIFO LLENA, 0:no se completo la mision
    q->Xdata[q->tail]=x;
	q->Ydata[q->tail]=y;
	q->Pdata[q->tail]=p;
	q->nLibres--;q->nOcupados++;
    q->tail = (q->tail + 1) % SIZE_MAX_FIFO;
return TRUE;
}//fin enqueue++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned char dequeue(FIFO_VFD *q,struct VFD_DATA *v ) {	
    if(is_empty_Queue(q)) 
           return FALSE;//0:No se completo la operacion
    v->x=q->Xdata[q->head];
	v->y=q->Ydata[q->head];
	v->p=q->Pdata[q->head];
	#if (debug_level1==1)
	   q->Xdata[q->head]=0;
	   q->Ydata[q->head]=0;
	   q->Pdata[q->head]=0;
	#endif   
	q->nLibres++;q->nOcupados--;
    q->head = (q->head + 1) % SIZE_MAX_FIFO;
return TRUE;
}//fin de queue+++++++++++++++++++++++++++++++++

/*  Control de Display de VFD de despliegue por thread  */
void* SubProceso_Tx_VFD(void* arg) {
    FIFO_VFD* q = (FIFO_VFD*)arg;
	struct VFD_DATA data;
	unsigned char estado124;
	printf("\n       Proceso  Transmissor a VFD Iniziando");
	while(!ret){
	 switch(estado124){
	   case 1:NoErrorOK();
	          if(vfd.config.bits.init_VFD==0)
	               pthread_cond_wait(&cond_init_TX_VFD,&mutex_init_VFD);//esperamos cond y liberamos mutex	
              estado124++;break;//start para iniciar el proceso
	   case 2:q->v->config.bits.Proc_VFD_Tx_running=TRUE;estado124++;break;
	   case 3:printf("\n       Esperando recurso Queue");
	          if(vfd.config.bits.init_VFD==0)
	             pthread_mutex_lock(&mutex_init_VFD);	 
			   estado124++;break;
	   case 4:NoErrorOK();estado124++;break;
	   case 5:if(dequeue(q,&data)){estado124++;}
	          else{if(q->v->config.bits.init_VFD) //todavia no acaba de init el vfd ??
			            estado124=10;//se termino de inizializar el VFD el hilo padre ha muerto
				   else{pthread_mutex_unlock(&mutex_init_VFD);
				        estado124=3;}}
			  break;
	   case 6:printf("\n       Procesando dato:%x,%x,%x",data.x,data.y,data.p);
	          pthread_mutex_unlock(&mutex_init_VFD);
	          NoErrorOK();
			  estado124++;
			  break;
	   case 7:estado124++;break;
	   case 8:estado124=5;break;//ciclo de nuevo
       case 10:q->v->config.bits.Proc_VFD_Tx_running=FALSE;
	           ret=TRUE;estado124=0;break;
	   default:estado124=1;break;}}//fin switch y while
return NULL;
}//fin del subproceso de envio de datos al display+++++++++++++

//Proceso  unico de padre unico  y sin instancias
void* Init_VFD(void* arg){  
struct _DISPLAY_VFD_* vfd1=(struct _DISPLAY_VFD_*)arg;
unsigned char ret=0,estado;
const unsigned char SIZE_CMD=7;//numero de comandos
const unsigned char s[7]={0x1BU,0x40U,0x1FU,0x28U,0x67U,0x01U,FONTSIZE2};
unsigned char i=0;
#if (debug_level1==1) 
   printf("\n       Iniziando mutex y semaforos");
#endif  
  if(vfd1->config.bits.init_VFD){
	   errorCritico("ya esta inizializado Proceso, Error de duplicacion");}	   
 while(!ret){
	switch(estado){
		case 1:pthread_mutex_init(&mutex_init_VFD,NULL);
		       pthread_cond_init(&cond_init_TX_VFD,NULL);
			    NoErrorOK();
			    estado++;break;
		case 2:printf("\n       Creando Hilo Transmisor");
		       switch(pthread_create(&Proc_Tx_VFD,NULL,SubProceso_Tx_VFD,&vfdtx)){//ret==0 :all OK	
				case 0:NoErrorOK();break;//todo ok
				case EAGAIN:errorCritico("Recursos insuficientes,Error Proc Tx VFD");break;
				case EINVAL:errorCritico("Arg invalidos,Error de Proc Tx VFD");break;
				case EPERM:errorCritico("Permisos Insuficientes,Error Proc Tx VFD");break;
				default:errorCritico("Error desconocido Proc Tx VFD");break;}
		       pthread_detach(Proc_Tx_VFD);//el hilo ahora es independiente
			   estado++;break;
	    case 3:printf("\n       LLenando los FIFOs para Transmitir");
		       pthread_cond_signal(&cond_init_TX_VFD);estado++;break;//start hilo transmisor
		case 4:pthread_mutex_lock(&mutex_init_VFD);estado++;break;
		case 5:if(VFDcommand(s[i]))estado=7;else{estado++;}break; // init display  ESC@= 1BH,40H
        case 6:pthread_mutex_unlock(&mutex_init_VFD);estado=4;break;
		case 7:if(++i<SIZE_CMD)estado=5;else{estado++;}break;
		case 8:pthread_mutex_unlock(&mutex_init_VFD);estado++;break;
        case 9:vfd.config.bits.init_VFD=TRUE;
		       estado=0;ret=TRUE;break;
		default:estado=1;break;}}//fin switch while 
#if (debug_level1==1) 
    NoErrorOK();
    printf("\n       Sub Proceso Init Terminado");
	NoErrorOK();
#endif  
return NULL;
}//fin init VFD -------------------------------------------------------------------



void Terminar_subProcesos(void){
    //pthread_join(Proc_Tx_VFD,NULL);
	//pthread_mutex_destroy(&q->lock);
}//terminar subprocesos+++++++++++++++++++++++++


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
//volatile unsigned char n=0;	
//static unsigned char control;
auto unsigned char ret=0;
    
    //if(!(vfd.x.ncount<SIZE_BUFFER6))
    //	 return FALSE;//esta muy llena la FIFO, espera un poco
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
    	case _BOLD_ :break;
    	default:break;}
     //n=vfd.x.appendByte(x,&vfd.x);deprecated
	 //n+=vfd.y.appendByte(y,&vfd.y);deprecated
	 //n+=vfd.p.appendByte(p,&vfd.p);deprecated
     ret=enqueue(&vfdtx,x,y,p);
	 //if(n==3){//fifo llena
	   //   ret=TRUE;}deprecated
return ret;//ret;
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
