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
    int turnaroundTime;
    int remainingTime;
    int numberInterruptions;
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

        if (i > 0 && i <= n1) //Procesos CPU bound (pocas interrupiones de mucho tiempo)
        {
            int n_itrp = rand() % 4 + 1; //Número de interrupciones aleatorio entre 1 y 4
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
        else if(i > n1) //Procesos I/O bound (muchas interrupciones de poco tiempo)
        {
            int n_itrp = rand() % 8 + 4; //Número de interrupciones aleatorio entre 4 y 8
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

//Funcion para simular un scheduler con algoritmo Round Robin 
void roundRobin(struct Process pro[], int n, int quantum)
{
    int time = 0;
    int flag, totalWaitTime = 0, totalResponseTime = 0, totalTurnaroundTime = 0;

    //Ejercutar el round robin hasta que todos los procesos hayan terminado
    while(1)
    {
        flag = 0; //Se mantiene 0 si todos los procesos terminan

        //Transversar por cada proceso
        for(int i=0; i<n; i++)
        {
            // Si el proceso ya se completó, salta a la siguiente iteración.
            if (pro[i].state == 2)
            {
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

                
                int flag2=0;
                for(int j=1; j<=quantum; j++) //Cada segundo del quantum
                {
                    time++; //Se aumenta un segundo al tiempo total
                    pro[i].remainingTime--; //Se disminuye el remaining time en un segundo 

                    if(pro[i].remainingTime==0)
                    {
                        // Registra el tiempo de finalización del proceso.
                        pro[i].exitTime = time;

                        // Calcula el tiempo de espera del proceso.
                        pro[i].waitingTime = pro[i].exitTime - pro[i].arrivalTime - pro[i].burstTime;

                        //Calcula el tiempo de turnaround 
                        pro[i].turnaroundTime = pro[i].exitTime - pro[i].arrivalTime;
                        
                        // Calcula los tiempos de espera y de respuesta promedio de todos los procesos.
                        totalWaitTime += pro[i].waitingTime;
                        totalResponseTime += pro[i].responseTime;
                        totalTurnaroundTime += pro[i].turnaroundTime;
                        
                        //Cambiar de estado
                        pro[i].state=2;

                        break;
                    }

                    /*Chequea si es que ocurre una interrupcion*/
                    for (int k=0; k<pro[i].numberInterruptions; k++) //Cada interrupción 
                    {
                        if(pro[i].burstTime - pro[i].remainingTime == pro[i].interrupts[k].when) //Si el tiempo de la interrupción es el mismo del tiempo actual
                        {
                            time+=pro[i].interrupts[k].duration; //Se ocupa de la interrupcion
                            flag2=1;
                            break;
                        }
                    }
                    if(flag2==1)
                    {
                        break;
                    }
                }
            }
            // Si el tiempo de llegada del proceso es mayor que el tiempo actual
            else
            {
                //Si los procesos anteriores ya iniciaron y el que se encuentra ahora todavía no llega la cola de ready
                if(i>0 && pro[i-1].state==1 && pro[i].state==0)
                {
                    i=-1;
                }
                //Se avanza el tiempo hasta el tiempo de llegada del proceso.
                else
                {
                    time++;
                    i--;
                } 
            }
        }
        // Si ningún proceso se procesó en esta iteración,
        // significa que todos los procesos se han completado.
        if(flag==0)
        {
            break;
        }
    }

    // Calcula el tiempo de respuesta promedio.
    float avgResponseTime = (float)totalResponseTime / (float)n;

    // Calcula el tiempo de espera promedio.
    float avgWaitTime = (float)totalWaitTime / (float)n;

    //Calcula el turnaround time promedio
    float avgTurnaroundTime = (float)totalTurnaroundTime /(float)n;

    //Calcula el throughput o rendimiento
    float throughputTime = (float)n/(float)time;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de Espera\t \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t %d\t\t %d\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime, pro[i].exitTime , pro[i].waitingTime);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f", avgWaitTime);
    printf("\nTiempo de Turnaround Promedio = %.2f", avgTurnaroundTime);
    printf("\nThroughtput = %.2f\n", throughputTime);
}


//Funcion para simular un scheduler con algoritmo First Come First Served
void FCFS(struct Process pro[], int n)
{
    int time = 0;
    int flag, totalWaitTime = 0, totalResponseTime = 0, totalTurnaroundTime = 0;

    //Ejercutar el round robin hasta que todos los procesos hayan terminado
    while(1)
    {
        flag = 0; //Se mantiene 0 si todos los procesos terminan

        //Transversar por cada proceso
        for(int i=0; i<n; i++)
        {
            // Si el proceso ya se completó, salta a la siguiente iteración.
            if (pro[i].state == 2)
            {
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

                
                int flag2=0;
                for(int j=1; j<=pro[i].burstTime; j++) //Cada segundo del quantum
                {
                    time++; //Se aumenta un segundo al tiempo total
                    pro[i].remainingTime--; //Se disminuye el remaining time en un segundo 

                    if(pro[i].remainingTime==0)
                    {
                        // Registra el tiempo de finalización del proceso.
                        pro[i].exitTime = time;

                        // Calcula el tiempo de espera del proceso.
                        pro[i].waitingTime = pro[i].exitTime - pro[i].arrivalTime - pro[i].burstTime;

                        //Calcula el tiempo de turnaround 
                        pro[i].turnaroundTime = pro[i].exitTime - pro[i].arrivalTime;
                        
                        // Calcula los tiempos de espera y de respuesta promedio de todos los procesos.
                        totalWaitTime += pro[i].waitingTime;
                        totalResponseTime += pro[i].responseTime;
                        totalTurnaroundTime += pro[i].turnaroundTime;
                        
                        //Cambiar de estado
                        pro[i].state=2;

                        break;
                    }

                    /*Chequea si es que ocurre una interrupcion*/
                    for (int k=0; k<pro[i].numberInterruptions; k++) //Cada interrupción 
                    {
                        if(pro[i].burstTime - pro[i].remainingTime == pro[i].interrupts[k].when) //Si el tiempo de la interrupción es el mismo del tiempo actual
                        {
                            time+=pro[i].interrupts[k].duration; //Se ocupa de la interrupcion
                            flag2=1;
                            break;
                        }
                    }
                    if(flag2==1)
                    {
                        break;
                    }
                }
            }
            // Si el tiempo de llegada del proceso es mayor que el tiempo actual
            else
            {
                
                time++;
                i--;
            
            }
        }
        // Si ningún proceso se procesó en esta iteración,
        // significa que todos los procesos se han completado.
        if(flag==0)
        {
            break;
        }
    }

    // Calcula el tiempo de respuesta promedio.
    float avgResponseTime = (float)totalResponseTime / (float)n;

    // Calcula el tiempo de espera promedio.
    float avgWaitTime = (float)totalWaitTime / (float)n;

    //Calcula el turnaround time promedio
    float avgTurnaroundTime = (float)totalTurnaroundTime /(float)n;

    //Calcula el throughput o rendimiento
    float throughputTime = (float)n/(float)time;

    // Imprime los resultados.
    printf("\nProceso\t Tiempo de Llegada\t Tiempo de Ejecución\t Tiempo de Respuesta\t Tiempo Final\t Tiempo de Espera\t \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t %d\t\t\t %d\t\t\t %d\t\t\t %d\t\t %d\n", pro[i].id, pro[i].arrivalTime, pro[i].burstTime, pro[i].responseTime,pro[i].exitTime , pro[i].waitingTime);
    }
    printf("\nTiempo de Respuesta Promedio = %.2f", avgResponseTime);
    printf("\nTiempo de Espera Promedio = %.2f", avgWaitTime);
    printf("\nTiempo de Turnaround Promedio = %.2f", avgTurnaroundTime);
    printf("\nThroughtput = %.2f", throughputTime);
}

void SJF(struct Process pro[], int n) 
{
    
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
