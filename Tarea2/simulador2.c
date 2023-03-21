#include <stdio.h>
#include <time.h>

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
    struct Interupts *interrupts;
};

// Estructura para las interrupciones
struct Interupts
{
    int id;
    int duration;
    int when; //en que segundo del proceso va a ocurrir la interrupcion
};

//Funcion creacion de procesos 
void createProcess(struct Process pro[], int n, int condition)
{
    int n1 = 0;

    if (condition == 1) //50% CPU bound y 50% I/O bound (literal a)
    {
        n1 = n*0.5;
    }
    else if (condition == 2) //90% CPU bound y 10% I/O bound (literal b)
    {
        n1 = n*0.9;
    }
    else if (condition == 3) //10% CPU bound y 90% I/O bound (literal c)
    {
        n1 = n*0.1;
    }
    else{
        printf("Warning no se asigno la razon de bounds");
    }

    n1-=1;

    for(int i=0; i<n; i++) //Llena la informacion de cada proceso (uno a uno)
    {
        //Asignar el id
        pro[i].id = i+1;
        //Asignar un tiempo aleatorio del arrivalTime entre 1 y 10
        pro[i].arrivalTime = rand() % 10 + 1;
        //Para cada proceso de asigna un burst time aleatorio entre 10 y 50
        pro[i].burstTime = rand() % 50 + 10;
        //se asigno -1 al response time para saber si hay algun un error
        pro[i].responseTime = -1;

        if (i > 0 && i <= n1) //Procesos CPU bound (pocas interrupiones de poco tiempo)
        {
            int n_itrp = rand() % 4 + 1; //Número de interrupciones aleatorio entre 1 y 4
            struct Interupts itrp[n_itrp]; //Arreglo de interrupciones
            int whenTimes[n_itrp]; //Areglo de tiempos de interrupcion que ya estan

            //Se asigna un valor aleatorio de tiempo a cada interrupcion y su id
            for(int j=0; j<n_itrp; j++){
                itrp[j].id = j+1;
                itrp[j].duration = rand() % 3 + 1; //Entre 1 y 3
            }

            //Se asigna aleatoreamente cuando va a ocurrir las interrupciones
            for(int j=0; j<n_itrp; j++){
                int whenItrp = rand() % pro[i].burstTime + 1; //Entre 1 y brustTime del proceso
                int whenExists = 0;
                for (int k = 0; k < j; k++){
                    if(whenTimes[k] == whenItrp){
                        whenExists = 1;
                        break;
                    }
                }

                if (!whenExists){
                    whenTimes[j] = whenItrp;
                    itrp[j].when = whenItrp;
                }
                else{
                    i--;
                }
            }

            pro[i].interrupts = malloc(n_itrp * sizeof(struct Interupts)); 
            pro[i].interrupts = itrp;

        }
        else if(i > n1) //Procesos I/O bound (muchas interrupciones de mas tiempo)
        {
            int n_itrp = rand() % 8 + 4; //Número de interrupciones aleatorio entre 4 y 8
            struct Interupts itrp[n_itrp]; //Arreglo de interrupciones
            int whenTimes[n_itrp]; //Areglo de tiempos de interrupcion que ya estan

            //Se asigna un valor aleatorio de tiempo a cada interrupcion y su id
            for(int j=0; j<n_itrp; j++){
                itrp[j].id = j+1;
                itrp[j].duration = rand() % 6 + 3; //Entre 3 y 6
            }

            //Se asigna aleatoreamente cuando va a ocurrir las interrupciones
            for(int j=0; j<n_itrp; j++){
                int whenItrp = rand() % pro[i].burstTime + 1; //Entre 1 y brustTime del proceso
                int whenExists = 0;
                for (int k = 0; k < j; k++){
                    if(whenTimes[k] == whenItrp){
                        whenExists = 1;
                        break;
                    }
                }

                if (!whenExists){
                    whenTimes[j] = whenItrp;
                    itrp[j].when = whenItrp;
                }
                else{
                    i--;
                }
            }

            pro[i].interrupts = malloc(n_itrp * sizeof(struct Interupts)); 
            pro[i].interrupts = itrp;
        }
        else{
            printf("Error, asignar la razon de bounds");
        }
    }
}

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
        flag = 0; // indica que no hay procesos que ejecutar

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
                    printf("Proceso %d: Tiempo %d - %d\n", pro[i].id, time, time + remainingTime[i]);
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

        // Ejecuta el SJF hasta que todos los procesos hayan terminado.
        for (i = 0; i < n; i++) 
        {
            //Ordenar arreglo por el arrival time
            quicksort(pro, 0, n - 1, 1);

            // Si el proceso ya se completo, salta a la siguiente iteracion.
            if (pro[i].remainingTime == 0)
            {
                continue;
            }

            flag=1;

            // Si el tiempo actual es menor que el tiempo de llegada del proceso,
            // espera hasta que llegue el proceso.
            if (pro[i].arrivalTime <= time)  
            {
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
            //Ordenar arreglo por el burst time
            quicksort(pro, 0, n - 1, 2);

            // Si el proceso ya se completo, salta a la siguiente iteracion.
            if (pro[i].remainingTime == 0)
            {
                continue;
            }

            flag=1;

            // Si el tiempo actual es menor que el tiempo de llegada del proceso,
            // espera hasta que llegue el proceso.
            if (pro[i].arrivalTime <= time)  
            {
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

int main()
{

    int i, n, quantum;

    // Ingreso de datos: numero de procesos, el burst time. el arrival time de cada proceso y el quantum
    printf("Ingrese el número de proceso: ");
    scanf("%d", &n);

    struct Process pro[n];

    srand(time(NULL)); //Instrucción que inicializa el generador de números aleatorios
    
    /*
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
    */

   //Crear procesos
    createProcess(pro, n, 0);
    
    printf("Ingrese el quantum para el algoritmo de round robin: ");
    scanf("%d", &quantum);

    // Ordenamiento del arreglo segun su tiempo de llegada
    quicksort(pro, 0, n - 1, 1);

    //Scheduling con Round Robin
    printf("\nRound Robin Scheduler: \n");
    roundRobin(pro, n, quantum);

    //Scheduling con FIFO 
    printf("\nFirst Come First Served Scheduler: \n");
    FCFS(pro,n);

    //Scheduling con SJF
    printf("\nShortest Job First Scheduler: \n");
    SJF(pro, n);

    return 0;
}
