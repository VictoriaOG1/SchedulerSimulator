#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESOS 10 // Número máximo de procesos en la cola

struct proceso {
    int id_proceso;
    int tiempo_llegada;
    int tiempo_cpu;
    int tiempo_restante;
    int tiempo_espera;
};

struct proceso cola[MAX_PROCESOS]; // Cola de procesos
int front = 0, rear = 0; // Índices del frente y el final de la cola
int tiempo_quantum = 2; // Quantum de tiempo para Round Robin

void encolar(struct proceso p) {
    if (rear == MAX_PROCESOS) {
        printf("La cola está llena\n");
        exit(1);
    }
    cola[rear++] = p;
}

struct proceso desencolar() {
    if (front == rear) {
        printf("La cola está vacía\n");
        exit(1);
    }
    return cola[front++];
}

bool hay_procesos_restantes() {
    for (int i = front; i < rear; i++) {
        if (cola[i].tiempo_restante > 0) {
            return true;
        }
    }
    return false;
}

int main() {
    int n; // Número de procesos a simular
    printf("Ingrese el número de procesos a simular: ");
    scanf("%d", &n);
    
    // Inicializar los procesos y agregarlos a la cola
    for (int i = 0; i < n; i++) {
        struct proceso p;
        p.id_proceso = i + 1;
        printf("Ingrese el tiempo de llegada del proceso %d: ", p.id_proceso);
        scanf("%d", &p.tiempo_llegada);
        printf("Ingrese el tiempo de CPU del proceso %d: ", p.id_proceso);
        scanf("%d", &p.tiempo_cpu);
        p.tiempo_restante = p.tiempo_cpu;
        p.tiempo_espera = 0;
        encolar(p);
    }
    
    // Simulación de Round Robin
    int tiempo_actual = 0;
    while (hay_procesos_restantes()) {
        struct proceso p = desencolar();
        if (p.tiempo_restante > tiempo_quantum) {
            p.tiempo_restante -= tiempo_quantum;
            tiempo_actual += tiempo_quantum;
            p.tiempo_espera += tiempo_actual - p.tiempo_llegada - p.tiempo_cpu;
            encolar(p);
        } else {
            tiempo_actual += p.tiempo_restante;
            p.tiempo_espera += tiempo_actual - p.tiempo_llegada - p.tiempo_cpu;
            p.tiempo_restante = 0;
            printf("Proceso %d terminado. Tiempo de espera: %d\n", p.id_proceso, p.tiempo_espera);
        }
    }
    
    return 0;
}