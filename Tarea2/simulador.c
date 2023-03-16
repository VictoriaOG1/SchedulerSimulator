#include <stdio.h>
#include <time.h>

srand(time(NULL));

//funcion para literal b y c
int calcular_90_10(int num_procesos) {
    int index;
    index = (num_procesos * 0.9) - 1;
    return index;
}

//funcion para literal a
int calcular_50_50(int num_procesos) {
    int index
    index = (num_procesos/2) - 1;
    return index;
}

//ordenar aleatoriamente los procesos
void shuffle(struct Process *arreglo){
    int indice_aleatorio, temporal;
    for (int i = tamano - 1; i > 0; i--) {
        indice_aleatorio = rand() % (i + 1); 
        temporal = arreglo[i]; 
        arreglo[i] = arreglo[indice_aleatorio];
        arreglo[indice_aleatorio] = temporal;
    }
}


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
    int typeBound; 
};
// Intercambia las direcciones de memoria dentro del array
void swap(struct Process* a, struct Process* b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}
// Divide una particion del array sobre un pivote y mueve los mayores a este a un lado y menores al otro
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
// Utiliza recursividad para ordenar el arreglo de procesos (para asegurarnos que esten en order de llegada)
void quicksort(struct Process arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
 
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

//Funcion para simular un scheduler con algoritmo Round Robin 
void roundRobin(struct Process pro[], int n, int quantum)
{
    int time = 0;
    int interruptsQueue[10] = [0,1,1,2,2,0,1,1,2,2] //se usara obtener la probabilidad de interrupciones I/O o CPU
    int positionInteQueue;
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
                
                //calcula la probabilidad de interrupcion
                
                if(typeBound == 0){
                    
                    positionInteQueue = rand() % 3;
                    if(interruptsQueue[positionInteQueue] == 0){
                        //interuptir el proceso mandarlo a la cola de bloqueado
                        //corer un x tiempo random 0(sin el 0) - 0.5 (hacer una condicion de 
                            //que si ya se corrio el tiempo ponerlo como proceso terminado y 
                            //moverlo a la cola de ready)
                    }
                }
                if(typeBound == 1){
                    positionInteQueue = rand() % 3;
                    if(interruptsQueue[positionInteQueue] == 1){
                        //interuptir el proceso mandarlo a la cola de bloqueado
                        //corer un x tiempo random 0(sin el 0) - 1 (hacer una condicion de 
                            //que si ya se corrio el tiempo ponerlo como proceso terminado y 
                            //moverlo a la cola de ready)
                    }
                }
                else
                {
                    printf("Error, no se asignado el tipo de Bound\n")
                }


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
                    printf("Proceso %d: Tiempo %d - %d\n", pro[i].id, time, time + remainingTime[i]);
                    time += remainingTime[i];
                    remainingTime[i] = 0;

                    // Registra el tiempo de finalizacion del proceso.
                    pro[i].exitTime = time;
                    //Calcula el tiempo de servicio del proceso 
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

int main()
{
    int i, n, quantum;

    //Ingreso de datos: numero de procesos, el burst time. el arrival time de cada proceso y el quantum
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

    //dependiendo del literal calcular el 90_10, 10_90 o calcular_50_50
    //shuffle(pro[n]);
    //asignar CPU o IO depnediendo del literal y en base al indice calculardo
    //en el comentario anterior.
    
    printf("Ingrese el quantum para el algoritmo de round robin: ");
    scanf("%d", &quantum);

    //Ordenamiento del arreglo segun su tiempo de llegada
    quicksort(pro,0, n-1);

    printf("\nRound Robin Scheduler: \n");
    //Se realiza el scheduling
    roundRobin(pro, n, quantum);

    return 0;
}
