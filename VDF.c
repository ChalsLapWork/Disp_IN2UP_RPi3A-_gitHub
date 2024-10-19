#include <pthread>
#include "VDF.h"


typedef struct{
    pthread_t hilo_id;//id del hilo
    int isAlive;//esta vivo?
}HiloInfo;


