#include <stdio.h>

// Estructura para representar un Process
struct Process {
    int id;             
    int arrivalTime;  
    int brustTime;      
    int state;         
    int responseTime;   
    int waitingTime;      
    int exitTime;      
};

void roundRobin(struct Process pro[], int n, int quantum) {
    int actualTime = 0;
    int totalTime = 0;
    int i, j, flag = 0;
    float waitAverage = 0, exitAverage = 0;

    struct Process temp[n];
    for (i = 0; i < n; i++) {
        temp[i] = pro[i];
        temp[i].state = 0;
    }

    while (1) {
        flag = 0;
        for (i = 0; i < n; i++) {
            if (temp[i].state == 0 && temp[i].arrivalTime <= actualTime) {
                temp[i].state = 1; //proceso listo
            }
            if (temp[i].state == 1) {
                flag = 1;   //todavia hay procesos por ejecutar
                if (temp[i].brustTime > quantum) {
                    totalTime += quantum;
                    temp[i].brustTime -= quantum;
                    actualTime += quantum;
                    temp[i].state = 2; //proceso en ejecucion
                } else {
                    totalTime += temp[i].brustTime;
                    actualTime += temp[i].brustTime;
                    temp[i].brustTime = 0;
                    temp[i].state = 3; //proceso completado
                    temp[i].exitTime = actualTime - temp[i].arrivalTime;
                    temp[i].waitingTime = temp[i].exitTime - temp[i].responseTime;
                    waitAverage += temp[i].waitingTime;
                    exitAverage += temp[i].exitTime;
                }
            }
        }
        if (flag == 0) {
            break; //todos los procesos completado
        }
    }
    exitAverage /= n;
    waitAverage /= n;

    // Impresión de la tabla
    printf("\nProcess\tLlegada\tCPU\tState\t\tRespuesta\tEspera\tRetorno\n");
        for (i = 0; i < n; i++) {
            printf("%d\t%d\t%d\t", temp[i].id, temp[i].arrivalTime, pro[i].brustTime);
            if (temp[i].state == 0) {
                printf("No iniciado\t\t-\t-\t-\n");
            } else if (temp[i].state == 1) {
                printf("En espera\t\t-\t-\t-\n");
            } else if (temp[i].state == 2) {
                printf("En ejecución\t\t%d\t-\t-\n", actualTime - temp[i].arrivalTime);
                temp[i].responseTime = actualTime - temp[i].arrivalTime;
            } else {
                printf("Completado\t\t%d\t%d\t%d\n", temp[i].responseTime, temp[i].waitingTime, temp[i].exitTime);
            }
        }
    printf("\nTiempo promedio de espera: %.2f\n", waitAverage);
    printf("Tiempo promedio de retorno: %.2f\n", exitAverage);
}
    

int main() {
    int i, n, quantum;
    printf("Ingrese el número de Pro: ");
    scanf("%d", &n);
    
    struct Process pro[n];
    
    for (i = 0; i < n; i++) {
        printf("Ingrese el tiempo de llegada para el Process %d: ", i + 1);
        scanf("%d", &pro[i].arrivalTime);
    
        printf("Ingrese el tiempo de CPU para el Process %d: ", i + 1);
        scanf("%d", &pro[i].brustTime);
    
        pro[i].id = i + 1;
        pro[i].state = 0; //procesos no iniciados
    }
    
    printf("Ingrese el quantum para el algoritmo de Round Robin: ");
    scanf("%d", &quantum);
    
    roundRobin(pro, n, quantum);
    
    return 0;
}

