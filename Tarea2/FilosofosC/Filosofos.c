#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

pthread_mutex_t palillos[NUM_FILOSOFOS];

void* filosofo(void* arg) {
    int id = *((int*)arg);
    int izq = id;
    int der = (id + 1) % NUM_FILOSOFOS;

    while (1) {
	printf("Filosofo %d esta pensando\n", id);   
        // Tomar los palillos
        pthread_mutex_lock(&palillos[izq]);
        pthread_mutex_lock(&palillos[der]);
        printf("Filosofo %d esta comiendo\n", id); 
        sleep(1);
        // filosofo ha terminado de comer
        // Liberar los palillos
        pthread_mutex_unlock(&palillos[izq]);
        pthread_mutex_unlock(&palillos[der]);

        // Pensar
        printf("Filosofo %d esta pensando\n", id);
        sleep(1);
    }
    return NULL;
}

int main() {
    int i;
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

   
    // Inicializar mutex para cada palillo
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&palillos[i], NULL);
    }

    // Crear hilos para cada filosofo
    for (i = 0; i < NUM_FILOSOFOS; i++) {
    	ids[i] = i;  // Inicializar los IDs de los filósofos
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);  
    }

    // Esperar a que los hilos terminen
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Liberar los mutex
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&palillos[i]);
    }

    return 0;
}

