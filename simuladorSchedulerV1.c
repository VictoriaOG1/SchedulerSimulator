#include <stdio.h>
#include <string.h>

int reg[3] = {0,0,0};
// Estructura de nuestro proceso
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
};
// Intercambia las direcciones de memoria dentro del array
void swap(struct Process *a, struct Process *b)
{
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}
// Divide una particion del array sobre un pivote y mueve los mayores a este a un lado y menores al otro
int partition(struct Process arr[], int low, int high)
{
    int pivot = arr[high].arrivalTime;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].arrivalTime < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
// Utiliza recursividad para ordenar el arreglo de procesos (para asegurarnos que esten en orden de llegada)
void quicksort(struct Process arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

void contextChange(int index, int processingTime,struct Process pro[], int mem[])
{
    for (int r = 0; r < 3; r++)
    { // Se carga informacion del proceso en memoria
        memcpy(&reg[r], &mem[r + ((pro[index].id - 1) * 3)], sizeof(int));
        // printf("\nreg[%d]: %d copied over mem[%d]: %d",r,reg[r],r + ((pro[index].id - 1) * 3), mem[r + ((pro[index].id - 1) * 3)]);
    }
    for (int t = 0; t < processingTime; t++)
    { // Se realiza una operacion con esos datos
        reg[2] = reg[0] + reg[1];
        reg[0] = reg[2];
    }
    for (int r = 0; r < 3; r++)
    { // Se guarda el estado de la informacion en memoria
        memcpy(&mem[r + ((pro[index].id - 1) * 3)], &reg[r], sizeof(int));
        // printf("\nmem[%d]: %d copied over reg[%d]: %d",r + ((pro[index].id - 1) * 3), mem[r + ((pro[index].id - 1) * 3)],r,reg[r]);
    }
}
//Funcion para simular un scheduler con algoritmo Round Robin 
void roundRobin(struct Process pro[], int n, int quantum, int mem[])
{
    int time = 0;
    int i;
    int remainingTime[n];
    int flag, totalWaitTime = 0, totalResponseTime = 0, totalServiceTime;

    // Inicializa el tiempo restante del proceso como su tiempo de ejecucion.
    for (i = 0; i < n; i++)
    {
        remainingTime[i] = pro[i].burstTime;
    }

    // Ejecuta el Round Robin hasta que todos los procesos hayan terminado.
    while (1)
    {
        flag = 0; //indica que no hay procesos que ejecutar

        // Procesa todos los procesos uno por uno.
        for (i = 0; i < n; i++)
        {
            // Si el proceso ya se completo, salta a la siguiente iteracion.
            if (remainingTime[i] == 0)
            {
                continue;
            }

            // Marca que al menos un proceso esta siendo procesado en esta iteracion.
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
                // el proceso se completara en esta iteracion, asi que actualiza el tiempo restante.
                if (remainingTime[i] <= quantum)
                {
                    contextChange(i,remainingTime[i],pro,mem);
                    printf("Proceso %d: Tiempo %d - %d || Last calculated:%d\n", pro[i].id, time, time + remainingTime[i], reg[2]);
                    time += remainingTime[i];
                    remainingTime[i] = 0;

                    // Registra el tiempo de finalizacion del proceso.
                    pro[i].exitTime = time;
                    // Calcula el tiempo de servicio del proceso
                    pro[i].serviceTime = pro[i].exitTime - pro[i].arrivalTime;
                    // Calcula el tiempo de espera del proceso.
                    pro[i].waitingTime = pro[i].serviceTime - pro[i].burstTime;
                    

                    // Calcula los tiempos de espera y de respuesta promedio de todos los procesos.
                    totalWaitTime += pro[i].waitingTime;
                    totalResponseTime += pro[i].responseTime;
                    totalServiceTime += pro[i].serviceTime;
                }

                // Si el tiempo restante es mayor que el quantum,
                // el proceso todavia no se ha completado, asi que se ejecuta durante el quantum.
                else
                {
                    contextChange(i,quantum,pro,mem);
                    printf("Proceso %d: Tiempo %d - %d || Last calculated:%d\n", pro[i].id, time, time + quantum, reg[2]);
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

        // Si ningun proceso se proceso en esta iteracion,
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

    //Calcula el índice de servicio
    float serviceIndex = (float)totalResponseTime / (float)time;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de servicio\t Tiempo de Espera\t \n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t %d\t\t\t %d\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime,pro[i].exitTime ,pro[i].serviceTime, pro[i].waitingTime);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f", avgWaitTime);
    printf("\nÍndice de servicio = %.2f \n", serviceIndex);
}

void FIFO()
{
    // sum = x + y;
    // x = sum;
}

int main()
{
    int i, n, quantum;

    //Ingreso de datos: numero de procesos, el burst time. el arrival time de cada proceso y el quantum
    printf("Ingrese el número de proceso: ");
    scanf("%d", &n);

    struct Process pro[n];
    int memory[3 * n];

    for(int i = 0; i<(3*n);i++){memory[i] = 1;} //Inicializa el espacio de memoria

    for (i = 0; i < n; i++)
    {
        printf("Ingrese el tiempo de llegada para el proceso %d: ", i + 1);
        scanf("%d", &pro[i].arrivalTime);

        printf("Ingrese el tiempo de CPU para el proceso %d: ", i + 1);
        scanf("%d", &pro[i].burstTime);

        pro[i].id = i + 1;
        pro[i].state = 0; // procesos no iniciados
        pro[i].responseTime = -1;
        // memory[i],memory[i+1],memory[i+2] = 1;
    }

    printf("Ingrese el quantum para el algoritmo de round robin: ");
    scanf("%d", &quantum);

    //Ordenamiento del arreglo segun su tiempo de llegada
    quicksort(pro, 0, n - 1);

    printf("\nOrdered by Arrival:\n");
    for (i = 0; i < n; i++)
    {
        printf("%d: Process %d\n", i, pro[i].id);
    }
    //Se realiza el scheduling
    roundRobin(pro, n, quantum, memory);

    return 0;
}
