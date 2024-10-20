
#define SIZE_BUF_TFT 5
#define SIZE_IO_TX_BUFFER 85 //ARRAY donde se guarda la direccion de la cabeza del paquete a enviar
#define SIZE_B 60//ARRAY donde se guarda los paquetes que se van a enviar

#define SIZE_FIFO_BOX 10
#define SIZE_FIFO SIZE_FIFO_BOX//30  //array size where the ADC's values are saved

#define SIZE_BARR_NUM 22//size fifo to displat numeros and position where menus has barra detection
#define SIZE_FIFO_CMD_DDS 30 //tamaño de la FIFO para transmitir comando DDS al touch

#define SIZE_BUFFER_TX 90 //BUFER DE TRANSMISION SERIAL ALA O
#define SIZE_BUFFER_DDS_FIFO 50
#define SIZE_TEMP_PIXEL  6//tamaño del buffer para guardar memoria de pixels del display DD
#define SIZE_DDS_PIXEL   40//para pintar el pixel en tiempo real
#define SIZE_DDS_ZOOM   2000
#define SIZE_X  24//array zoom repaint char pantalla 0
#define SIZE_Y  129//array zoom repaint char pantalla0

#define SIZE_SER_KEYPAD 10 //TAMAÑO de la fifo del serial del keypad de entraada
#define SIZE_KEYPAD_IN 10 //buffer de entrada de chars de serial de keypad
#define SIZE_BUFFER1  10U //buffers de rx y tx de serial a IOUP
#define SIZE_BUFFER2  5U//tamaño de rx y tx de i2c fifo memorias
#define SIZE_BUFFER5  20U//kEYPAD FLIP FLOP save keysrokes tamaño de buffer char que entran keypad serial 
#define SIZE_BUFFER6  10//1954U//FIFO de graficos box,char,rayas,puntos,pos,con SO
#define SIZE_BUFFER9  50 //fifo GUARDA LOS pixeles que estan fuera de la pantalla DDS porque se salieton del zoom,estan mas alla del valor desplegable actual al zoom actual
#define SIZE_BUFFER13 10 //FIFO para gestionar recurso VFD de los hilos
#define SIZE_BUFFER14 20//20=No modif streaming-block size transmision a la IOUP  
#define SIZE_BUFFER15 20 //20 no modificar FIFO guarda deteccion, para transmitirla por serial IOUP
#define SIZE_BUFFER16 5  //buffer de recursos de vfd de gestion de
#define SIZE_BOXES    20
#define SIZE_SYS_MONITOR 30
#define SIZE_TBC 15
#define SIZE_CHAR 5
#define SIZE_FEW  3


struct _VFD_{
 union _Config{
  unsigned char byte1;
  struct _Bits{
    unsigned char init_VFD:1;
    unsigned char init_menu:1;
    unsigned char BOX_enable:1;
    unsigned char x0:1;
    unsigned char x1:1;
    unsigned char x2:1;
    unsigned char x3:1;
    }bits;
  }config;
};


/*version 310322-1641 add reset genaral */
struct _FIFO_1byte_{//FIFO PARA UNA VARIABLE para un byte
    unsigned char *pop;//primer de la FIFO para salir
    unsigned char *head;//indice 0 del buffer
    unsigned char *tail;//ultimo indice del buffer
    unsigned char *push;//pointer buffer 2
	unsigned char (*popf)(unsigned char *n,struct _FIFO_1byte_ *s);//quitar el prmer elemento de la fifo DDS
	unsigned char (*appendByte)(unsigned char  n,struct _FIFO_1byte_ *s);//agregar byte al buffer de transmision
    void (*vaciarbuff)(unsigned char *p1,unsigned char *p2,unsigned short int size);
    void (*resetFIFO)(struct _FIFO_1byte_ *s,unsigned char *arr,unsigned short int size);
    unsigned short int ncount;//cuenta los nodos llenos en la fifo
    unsigned short int size; //size de la fifo
    unsigned char nLibres;//cantidad de nLibres
    unsigned char nOcupados;//cantidad de ocupados
};



struct _DISPLAY_VFD_{
	struct _FIFO_1byte_ x;//parametro 1
	struct _FIFO_1byte_ y;//parametro 2
	struct _FIFO_1byte_ p;//parametro 3
	struct _FIFO_func_  f1;//funciones para guardar lo que se grafica
	struct _box_control{
		 unsigned char boxs[SIZE_BOXES];
		 unsigned char box0;
		 unsigned char box; 
		 unsigned short int timer;//se activa  por timer y resetea el array
	   }box;
	union _Byte5_{
	   	  		unsigned char bytes1;
	   	  		struct{
	   	  			unsigned char FIFOonReset:1;//Las FIFOS estan reseteadas?? osea que esan en ceros y desbilitadas, esto para cambiar de contexto
	   	  			unsigned char DDSon:1;//indica si borramos registro de datos repetidos de DDS
	   	  			unsigned char TxBuffOFF:1;//buffer de TX vacio, para saber que ya se transmitio todo
	   	  			unsigned char finit_VFD:1;//flag init VFD indica si ya se init el VFD comandos de inizializacion
	   	  			unsigned char finit_Menu:1;//flag init Menu, enciende e inicializa los menus y el primer menu en pantalla
	   	  			unsigned char BOX_enable:1;
	   	  			unsigned char VDF_busy:1;//se estan mandando comandos  o posiciones
	   	  		    unsigned char ADC_DATO:1;
	   	  		}b;
	   	  	  }bits;
	struct _Vars_{
		unsigned char nbytes;//bytes a emitir
		unsigned char dat[DATOS_SIZE];
		unsigned short int timer;
		unsigned short int timer_us;
		unsigned long  int timer_ms;
		unsigned char index;
		unsigned char recurso;//quien ocupa este recurso.
		
//		unsigned char estado;//estado de: init_VFD,
	   }v;
    		
};//fin display VFD----------------------------------------------
