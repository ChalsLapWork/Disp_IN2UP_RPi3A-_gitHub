#include "string.h"


/* arr: es el array a limpiar, size es el tamaño del array, k es la kosntante
 * que se va allenar el array  */
void cleanArray(unsigned char *arr,unsigned char size,unsigned char k){
unsigned char  i;        
	for(i=0;i<size;i++)
	     *(arr+i)=k;
}//fin de clean array----------------------------------------------------------------
