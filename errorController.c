
#include "errorController.h"
#include "stdio.h"
#include "stdlib.h"


void errorCritico(char *c){  
    printf("\n\033[0;031m%s\033[0m\n",c);
    exit(-1);
}//fin error critico+++++++++++++++++++++++++++++++++++

void SiError(void){
    printf("\n\033[0;031m error\033[0m\n");
    exit(-1);
}//si error lineas rojas no OK

void NoErrorOK(void){
     printf("\r\033[0;32m[ ok ]\033[0m");
}// ok label in verde+++++++++++++++++++++++++++++