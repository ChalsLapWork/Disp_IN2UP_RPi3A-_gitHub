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
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "VFDisplay.h"

typedef struct Node{
  //unsigned char Xdata[SIZE_MAX_FIFO];
  //unsigned char Ydata[SIZE_MAX_FIFO];
  //unsigned char Pdata[SIZE_MAX_FIFO];
  struct VFD_DATA dato;
  struct Node *next;
}Node;//Nodo para crear las queues+++++++++++++++++++++++++++

typedef struct Queue{
  Node *head,*tail;
  int size;
  pthread_cond_t  cond_init_TX_VFD;//condicion de init VFD transmisor
  pthread_mutex_t mutex_init_VFD;//mutex para init VFD y transmisor
  #if(SIZE_MAX_FIFO<255)
    unsigned char nLibres;
	unsigned char nOcupados;
  #endif
  struct _DISPLAY_VFD_ *v;//pointer to the control general of vfd
}QueueTxVFD;


struct _DISPLAY_VFD_ vfd;
QueueTxVFD qVFDtx;//queue de transmision vfd 
void init_Queue_with_Thread(QueueTxVFD *q);
struct VFD_DATA dequeue(QueueTxVFD  *q);
void enqueue(QueueTxVFD *q,struct VFD_DATA dato1);
void* SubProceso_Tx_VFD(void* arg);

unsigned char  buffer6[SIZE_BUFFER6];//FIFO graficos con S.O, aqui guarda el dato
unsigned char  buffer7[SIZE_BUFFER6];//FIFO graficos con SO. aqui guarda el parametro=char|box|pos|
unsigned char  buffer8[SIZE_BUFFER6];//FIFO graficos con SO. aqui guarda el parametro numero 3


void init_queues(void){
	pthread_t Proc1_Init_VFD;//Proceso para inizializar el VFD
	init_FIFO_General_1byte(&vfd.x,&buffer6[0],SIZE_BUFFER6);
    init_FIFO_General_1byte(&vfd.y,&buffer7[0],SIZE_BUFFER6);
    init_FIFO_General_1byte(&vfd.p,&buffer8[0],SIZE_BUFFER6);
    printf("\n       Iniciando queueus");	  
    vfd.config.bytes1=0;//init all parameter into zero
    vfd.f1.append=vfd_FIFO_push;
	vfd.f1.pop=vfd_FIFO_pop;                                                                                                                                                                                                                                                                                                                                                                                                                      
	vfd.f1.resetFIFOS=vfd_FIFOs_RESET;
	qVFDtx.v=&vfd;//misma estructura en los dos lados,
	init_Queue_with_Thread(&qVFDtx);//fifos Transmisor data al Display
	vfd.config.bits.recurso_VFD_Ocupado=TRUE;//recurso ocupado, VFD nadie lo puede usar
	NoErrorOK();
	printf("\n       Creando Proceso Init VFD");
	switch(pthread_create(&Proc1_Init_VFD,NULL,Init_VFD,&qVFDtx)){
		case 0:NoErrorOK();break;
		case EAGAIN:errorCritico("Recursos insuficientes,Error de hilo init VFD");break;
		case EINVAL:errorCritico("Arg invalidos,Error de hilo init VFD");break;
		case EPERM:errorCritico("Permisos Insuficientes,Error de hilo init VFD");break;
		default:errorCritico("Error desconocido de hilo init VFD");break;}
	//pthread_detach(Proc_Init_VFD);//que muera sin monitor y libere recursos
    pthread_join(Proc1_Init_VFD,NULL);
	pthread_mutex_destroy(&qVFDtx.mutex_init_VFD);
    pthread_cond_destroy(&qVFDtx.cond_init_TX_VFD);
	printf("\n       Comenzamos las otras configuraciones");
	NoErrorOK();
}//fin init queue++++++++++




void init_Queue_with_Thread(QueueTxVFD *q){
      q->head=q->tail=NULL;
	  q->size=0;
	  q->nLibres=SIZE_MAX_FIFO;
	  q->nOcupados=0;
	  pthread_mutex_init(&q->mutex_init_VFD,NULL);//
	  pthread_cond_init(&q->cond_init_TX_VFD,NULL);
}//fin de init FIFO transmit VFD+++++++++++++++++++++++++
  


//encola regresa TRUE: si esta llena , FALSE: si esta vacia
void enqueue(QueueTxVFD *q,struct VFD_DATA dato1){
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->dato=dato1;
	new_node->next=NULL;
    pthread_mutex_lock(&q->mutex_init_VFD);
	if(q->tail==NULL){
		  q->head=new_node;
		  q->tail=new_node;}
	else{q->tail->next=new_node;
	     q->tail=new_node;}
	q->size++;	 
	q->nLibres--;q->nOcupados++;
    pthread_cond_signal(&q->cond_init_TX_VFD); // Notifica que la cola no está vacía
    pthread_mutex_unlock(&q->mutex_init_VFD);
}//fin enqueue++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct VFD_DATA dequeue(QueueTxVFD  *q) {
	pthread_mutex_lock(&q->mutex_init_VFD);
	while(q->size==0)
	    pthread_cond_wait(&q->cond_init_TX_VFD,&q->mutex_init_VFD);	//espera si la cola esta vacia
    Node *temp=q->head;
	struct VFD_DATA data=temp->dato;
	q->head=q->head->next;
    if(q->head==NULL)
	     q->tail=NULL;
    q->size--;
	free(temp);		 
    pthread_mutex_unlock(&q->mutex_init_VFD);
	q->nLibres++;q->nOcupados--;
return data;
}//fin de queue+++++++++++++++++++++++++++++++++

/*  Control de Display de VFD de despliegue por thread  */
void* SubProceso_Tx_VFD(void* arg) {//consumidor
    QueueTxVFD *q = (QueueTxVFD *)arg;
	struct VFD_DATA data;
	unsigned char estado124,mem[4];
	printf("\n       Proceso  Transmissor a VFD Iniciando");
	while(!vfd.config.bits.init_VFD||q->size>0){
	 switch(estado124){
	   case 1:NoErrorOK();
	          printf("\n       Tx, Lectura de init=%d",vfd.config.bits.init_VFD);
	          estado124++;NoErrorOK();break;//start para iniciar el proceso
	   case 2:q->v->config.bits.Proc_VFD_Tx_running=TRUE;estado124++;break;
	   case 3:data=dequeue(q);
	          estado124++;break;
	   case 4:if(Transmissor_a_VFD(data,&mem[0]))estado124=3;break;
	   default:estado124=1;break;}}//fin switch y while
	   q->v->config.bits.Proc_VFD_Tx_running=FALSE;
       printf("\n       Hilo TX VFD Apagado:%d",estado124);
 	   NoErrorOK();
       //sleep(500);	   
return NULL;
}//fin del subproceso de envio de datos al display+++++++++++++

//methodo que se usa en un hilo transmisor VFD+++++++++++++++++++++++
unsigned char Transmissor_a_VFD(struct VFD_DATA *v,unsigned char *mem){
unsigned char ret=0,estado1;
unsigned char *box1,*box0;
coordn16 coordenadas;
unsigned char pen,mode,ibox0,x1,y1,x2,y2;
const unsigned char DELAY_TIME=1;
enum edos {n0,n1,n2,CHARX,PUNTOX,POSX,DELAYUSX0,DELAYUSX1,DELAYMSX0,DELAYMSX1,
           n33,n34,n35,n54};
static union W7{//access word: 
	unsigned  short int wordx; //0xaabb //aa
	unsigned char byte[2];     //byte[0]=aa,byte[1]=bb
}w16;

   estado1=*(mem+0);
	   ret=*(mem+1);
	  box1=mem+2;
	  box0=mem+3;

      switch(estado1){//DRIVER DE VIDEO
    	  case 1:
		  default:break;}//fin estado principal-----------------------------------------      

}//transmisor de datos a VFD++++++++++++++++++++++++++++++++


//Proceso  unico de padre unico  y sin instancias
void* Init_VFD(void* arg){  //Proceso Productor
QueueTxVFD *q=(QueueTxVFD*)arg;
pthread_t Proc2_Tx_VFD;//Proceso Transmisor al VFD, para despliegue de pantalla
unsigned char ret=0,estado;
const unsigned char SIZE_CMD=7;//numero de comandos
const unsigned char s[7]={0x1BU,0x40U,0x1FU,0x28U,0x67U,0x01U,FONTSIZE2};
unsigned char i=0;
#if (debug_level1==1) 
   printf("\n       Iniziando mutex y semaforos");
#endif  
  if(q->v->config.bits.init_VFD){
	   errorCritico("ya esta inizializado Proceso, Error de duplicacion");}	   	   
 while(!ret){
	switch(estado){
		case 1:NoErrorOK();estado++;break;
		case 2:printf("\n       Creando Hilo Transmisor");
		       switch(pthread_create(&Proc2_Tx_VFD,NULL,SubProceso_Tx_VFD,&qVFDtx)){//ret==0 :all OK	
				case 0:NoErrorOK();break;//todo ok
				case EAGAIN:errorCritico("Recursos insuficientes,Error Proc Tx VFD");break;
				case EINVAL:errorCritico("Arg invalidos,Error de Proc Tx VFD");break;
				case EPERM:errorCritico("Permisos Insuficientes,Error Proc Tx VFD");break;
				default:errorCritico("Error desconocido Proc Tx VFD");break;}
			   estado++;break;
	    case 3:printf("\n       Init, comenzar a llenar los FIFOs Init para Transmitir");
			   NoErrorOK();estado++;break;
		case 4:if(VFDcommand(s[i]))estado++;break; // init display  ESC@= 1BH,40H
		case 5:if(++i<SIZE_CMD)estado=4;else{estado++;}break;
		case 6:vfd.config.bits.init_VFD=TRUE;estado++;break;
		case 7:pthread_cond_signal(&q->cond_init_TX_VFD);estado++;break;
        case 8:estado=0;ret=TRUE;break;
		default:estado=1;break;}}//fin switch while 
        pthread_join(Proc2_Tx_VFD,NULL);
	    printf("\n       Init Sub Proceso Init Terminado");
		NoErrorOK();
		//sleep(400);
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
struct VFD_DATA dato;
const unsigned char DELAY_TIME=1; 
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
	 dato.x=x;dato.y=y;dato.p=p;
     enqueue(&qVFDtx,dato);
	 //if(n==3){//fifo llena
	   //   ret=TRUE;}deprecated
return TRUE;//ret;
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
