#include <stdio.h>

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
    int remainingTime;
};

// Intercambia las direcciones de memoria dentro del array
void swap(struct Process *a, struct Process *b)
{
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

// Divide una particion del array sobre un pivote y mueve los mayores a este a un lado y menores al otro
int partition(struct Process arr[], int low, int high, int type)
{
    int i = (low - 1);

    if (type==1)
    {
        int pivot = arr[high].arrivalTime;
        
        for (int j = low; j <= high - 1; j++)
        {
            if (arr[j].arrivalTime < pivot)
            {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
    }
    else
    {
        int pivot = arr[high].burstTime;

        for (int j = low; j <= high - 1; j++)
        {
            if (arr[j].burstTime < pivot)
            {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
// Utiliza recursividad para ordenar el arreglo de procesos (para asegurarnos que esten en order de llegada)
void quicksort(struct Process arr[], int low, int high, int type)
{
    if (low < high)
    {
        int pi = partition(arr, low, high, type);

        quicksort(arr, low, pi - 1, type);
        quicksort(arr, pi + 1, high, type);
    }
}

// Funcion para simular un scheduler con algoritmo Round Robin
void roundRobin(struct Process pro[], int n, int quantum)
{
    int time = 0;
    int i;
    int flag, totalWaitTime = 0, totalResponseTime = 0, totalServiceTime;

    // Ejecuta el Round Robin hasta que todos los procesos hayan terminado.
    while (1)
    {
        flag = 0; // indica que no hay procesos que ejecutar

        // Procesa todos los procesos uno por uno.
        for (i = 0; i < n; i++)
        {
            // Si el proceso ya se completo, salta a la siguiente iteracion.
            if (pro[i].remainingTime == 0)
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
                if (pro[i].remainingTime <= quantum)
                {
                    printf("Proceso %d: Tiempo %d - %d\n", pro[i].id, time, time + pro[i].remainingTime);
                    time += pro[i].remainingTime;
                    pro[i].remainingTime = 0;

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
                    printf("Proceso %d: Tiempo %d - %d\n", pro[i].id, time, time + quantum);
                    time += quantum;
                    pro[i].remainingTime -= quantum;
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

    // Calcula el índice de servicio
    float serviceIndex = (float)totalResponseTime / (float)time;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de servicio\t Tiempo de Espera\t \n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t %d\t\t\t %d\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime, pro[i].exitTime, pro[i].serviceTime, pro[i].waitingTime);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f", avgWaitTime);
    printf("\nÍndice de servicio = %.2f \n", serviceIndex);
}


void FCFS(struct Process pro[], int n) 
{
    int time = 0;
    int i;
    int flag=0, totalWaitTime = 0, totalResponseTime = 0, totalServiceTime = 0;


    while(1)
    {
        flag = 0;

        // Ejecuta el FIFO hasta que todos los procesos hayan terminado.
        for (i = 0; i < n; i++) 
        {

            // Si el proceso ya se completo, salta a la siguiente iteracion.
            if (pro[i].remainingTime == 0)
            {
                continue;
            }

            // Si el tiempo actual es menor que el tiempo de llegada del proceso,
            // espera hasta que llegue el proceso.
            if (time < pro[i].arrivalTime) 
            {
                time = pro[i].arrivalTime;
            }

            // Marca que el proceso ya ha comenzado.
            pro[i].state = 1;

            // Registra el tiempo de respuesta del proceso.
            pro[i].responseTime = time - pro[i].arrivalTime;

            // Ejecuta el proceso durante su tiempo de burst.
            time += pro[i].burstTime;

            //Se completa el proceso y el remainingTime se convierte en 0
            pro[i].remainingTime = 0;

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

    // Calcula el índice de servicio
    float serviceIndex = (float)totalResponseTime / (float)time;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de servicio\t Tiempo de Espera\t \n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t %d\t\t\t %d\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime, pro[i].exitTime, pro[i].serviceTime, pro[i].waitingTime);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f", avgWaitTime);
    printf("\nÍndice de servicio = %.2f \n", serviceIndex);
    
}

void SJF(struct Process pro[], int n) 
{
    int time = 0;
    int i;
    int flag=0, totalWaitTime = 0, totalResponseTime = 0, totalServiceTime = 0;


    while(1)
    {
        flag = 0;

        // Ejecuta el SJF hasta que todos los procesos hayan terminado.
        for (i = 0; i < n; i++) 
        {

            // Si el proceso ya se completo, salta a la siguiente iteracion.
            if (pro[i].remainingTime == 0)
            {
                continue;
            }

            // Si el tiempo actual es menor que el tiempo de llegada del proceso,
            // espera hasta que llegue el proceso.
            if (time < pro[i].arrivalTime) 
            {
                time = pro[i].arrivalTime;
            }

            // Marca que el proceso ya ha comenzado.
            pro[i].state = 1;

            // Registra el tiempo de respuesta del proceso.
            pro[i].responseTime = time - pro[i].arrivalTime;

            // Ejecuta el proceso durante su tiempo de burst.
            time += pro[i].burstTime;

            //Se completa el proceso y el remainingTime se convierte en 0
            pro[i].remainingTime = 0;
            
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

    // Calcula el índice de servicio
    float serviceIndex = (float)totalResponseTime / (float)time;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de servicio\t Tiempo de Espera\t \n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t %d\t\t\t %d\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime, pro[i].exitTime, pro[i].serviceTime, pro[i].waitingTime);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f", avgWaitTime);
    printf("\nÍndice de servicio = %.2f \n", serviceIndex);
    
}

int main()
{
    int i, n, quantum;

    // Ingreso de datos: numero de procesos, el burst time. el arrival time de cada proceso y el quantum
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
        pro[i].state = 0; // Procesos no inicializados
        pro[i].responseTime = -1;
        // Inicializa el tiempo restante del proceso como su tiempo de ejecucion.
        pro[i].remainingTime = pro[i].burstTime;
    
    }

    printf("Ingrese el quantum para el algoritmo de round robin: ");
    scanf("%d", &quantum);

    // Ordenamiento del arreglo segun su tiempo de llegada
    quicksort(pro, 0, n - 1, 0);

    //Scheduling con Round Robin
    printf("\nRound Robin Scheduler: \n");
    roundRobin(pro, n, quantum);

    //Scheduling con FIFO 
    printf("\nFirst Come First Served Scheduler: \n");
    FCFS(pro,n);

    //Scheduling con SJF
    quicksort(pro, 0, n - 1, 1);
    printf("\nShortest Job First Scheduler: \n");
    SJF(pro, n);

    return 0;
}
