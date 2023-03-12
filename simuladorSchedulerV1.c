#include <stdio.h>

// Estructura para representar un Process
struct Process
{
    int id;
    int arrivalTime;
    int burstTime;
    int state;
    int responseTime;
    int waitingTime;
    int exitTime;
    int serviceTime;
    int indexService;
};

/*** FUNCIONES PARA ORNEDAMIENTO DE ARREGLO***/
void swap(struct Process* a, struct Process* b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Process arr[], int low, int high) {
    int pivot = arr[high].arrivalTime;
    int i = (low - 1);
 
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].arrivalTime < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(struct Process arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
 
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

//Función para simular un scheduler con algoritmo Round Robin 
void roundRobin(struct Process pro[], int n, int quantum)
{
    int time = 0;
    int i;
    int remainingTime[n];
    int flag, totalWaitTime = 0, totalResponseTime = 0;

    // Inicializa el tiempo restante del proceso como su tiempo de ejecución.
    for (i = 0; i < n; i++)
    {
        remainingTime[i] = pro[i].burstTime;
    }

    // Ejecuta el Round Robin hasta que todos los procesos hayan terminado.
    while (1)
    {
        flag = 0;

        // Procesa todos los procesos uno por uno.
        for (i = 0; i < n; i++)
        {
            // Si el proceso ya se completó, salta a la siguiente iteración.
            if (remainingTime[i] == 0)
            {
                printf("Proceso %d termino en tiempo %d\n", i, time);
                continue;
            }

            // Marca que al menos un proceso está siendo procesado en esta iteración.
            flag = 1;

            // Si el tiempo de llegada del proceso es menor o igual al tiempo actual,
            // se procesa el proceso durante el quantum de tiempo y se actualiza el tiempo restante.
            if (pro[i].arrivalTime <= time)
            {
                // Marca que el proceso ya ha comenzado.
                pro[i].state = 1;

                // Si es la primera vez que se ejecuta el proceso,
                // registra su tiempo de respuesta como el tiempo actual menos su tiempo de llegada.
                if (pro[i].responseTime == -1)
                {
                    pro[i].responseTime = time - pro[i].arrivalTime;
                }

                // Si el tiempo restante es menor o igual al quantum,
                // el proceso se completará en esta iteración, así que actualiza el tiempo restante.
                if (remainingTime[i] <= quantum)
                {
                    printf("Proceso %d: Tiempo %d - %d\n", pro[i].id, time, time + remainingTime[i]);
                    time += remainingTime[i];
                    remainingTime[i] = 0;

                    // Registra el tiempo de finalización del proceso.
                    pro[i].exitTime = time;
                    //Calcula el tiempo de servicio del proceso 
                    pro[i].serviceTime = pro[i].exitTime - pro[i].arrivalTime;
                    // Calcula el tiempo de espera del proceso.
                    pro[i].waitingTime = pro[i].serviceTime - pro[i].burstTime;
                    //Calculo los indices de servicio
		            pro[i].indexService = pro[i].burstTime / pro[i].serviceTime;

                    // Calcula los tiempos de espera y de respuesta promedio de todos los procesos.
                    totalWaitTime += pro[i].waitingTime;
                    totalResponseTime += pro[i].responseTime;
                }

                // Si el tiempo restante es mayor que el quantum,
                // el proceso todavía no se ha completado, así que se ejecuta durante el quantum.
                else
                {
                    printf("Proceso %d: Tiempo %d - %d\n", pro[i].id, time, time + quantum); 
                    time += quantum;
                    remainingTime[i] -= quantum;
                }
            }

            // Si el tiempo de llegada del proceso es mayor que el tiempo actual,
            // se avanza el tiempo hasta el tiempo de llegada del proceso.
            else
            {
                time++;
                i--;
            }
        }

        // Si ningún proceso se procesó en esta iteración,
        // significa que todos los procesos se han completado.
        if (flag == 0)
        {
            break;
        }
    }

    // Calcula el tiempo de respuesta promedio.
    float avgResponseTime = (float)totalResponseTime / (float)n;

    // Calcula el tiempo de espera promedio.
    float avgWaitTime = (float)totalWaitTime / (float)n;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de servicio\t Tiempo de Espera\t Indice de servicio\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t\t\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime,pro[i].exitTime ,pro[i].serviceTime, pro[i].waitingTime,pro[i].indexService);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f \n", avgWaitTime);
}

int main()
{
    //Variables
    int i, n, quantum;

    //Ingreso de datos: número de procesos, el burst time. el arrival time de cada proceso y el quantum
    printf("Ingrese el número de proceso: ");
    scanf("%d", &n);

    struct Process pro[n];

    for (i = 0; i < n; i++)
    {
        printf("Ingrese el tiempo de llegada para el proceso %d: ", i + 1);
        scanf("%d", &pro[i].arrivalTime);

        printf("Ingrese el tiempo de CPU para el proceso %d: ", i + 1);
        scanf("%d", &pro[i].burstTime);

        pro[i].id = i + 1;
        pro[i].state = 0; //Procesos no inicializados 
        pro[i].responseTime = -1;
    }

    printf("Ingrese el quantum para el algoritmo de round robin: ");
    scanf("%d", &quantum);

    //Ordenamiento del arreglo según su tiempo de llegada
    quicksort(pro,0, n-1);

    //Se realiza el scheduling
    roundRobin(pro, n, quantum);

    return 0;
}
