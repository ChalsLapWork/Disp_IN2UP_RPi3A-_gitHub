#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_SIZE 100

// Estructura de nodo para la cola
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Estructura de la cola con mutex y condiciones para sincronización
typedef struct Queue {
    Node* front;
    Node* rear;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
} Queue;

// Inicializa la cola
void init_queue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
}

// Agrega un elemento a la cola
void enqueue(Queue* queue, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    pthread_mutex_lock(&queue->mutex);
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
    pthread_cond_signal(&queue->not_empty); // Notifica que la cola no está vacía
    pthread_mutex_unlock(&queue->mutex);
}

// Extrae un elemento de la cola
int dequeue(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->size == 0) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex); // Espera si la cola está vacía
    }

    Node* temp = queue->front;
    int data = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    queue->size--;
    free(temp);
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

// Variables globales
Queue queue;
volatile int h1_terminado = 0;  // Bandera para indicar que h1 ha terminado

// Función del hilo h1 para generar datos
void* hilo_productor(void* arg) {
    for (int i = 0; i < 100; i++) {
        printf("Hilo h1 generando dato: %d\n", i);
        enqueue(&queue, i);  // Encola el dato en la cola
        usleep(10000);       // Simula un tiempo de procesamiento
    }
    h1_terminado = 1;  // Marca h1 como terminado
    pthread_cond_signal(&queue.not_empty); // Notifica a h2 en caso de estar esperando
    return NULL;
}

// Función del hilo h2 para consumir datos
void* hilo_consumidor(void* arg) {
    while (!h1_terminado || queue.size > 0) {
        int data = dequeue(&queue);  // Desencola un dato
        printf("Hilo h2 procesando dato: %d\n", data);
        usleep(50000);  // Simula un tiempo de procesamiento
    }
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2;

    init_queue(&queue);  // Inicializa la cola

    // Crea el hilo productor (h1)
    pthread_create(&hilo1, NULL, hilo_productor, NULL);

    // Crea el hilo consumidor (h2)
    pthread_create(&hilo2, NULL, hilo_consumidor, NULL);

    // Espera a que terminen ambos hilos
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    // Libera los recursos de la cola
    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.not_empty);

    printf("Todos los datos fueron procesados.\n");
    return 0;
}
