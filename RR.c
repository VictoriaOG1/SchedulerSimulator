#include<stdio.h>  
#include<conio.h>  
  
void main()  {  
    // inicializa las variables 
    int i, NOP, sum=0,count=0, y, quant, wt=0, tat=0, at[10], bt[10], temp[10];  
    float avg_wt, avg_tat;  
    printf(" Numero total de procesos en el sistema: ");  
    scanf("%d", &NOP);  
    y = NOP; // asigna numero de procesos a la variable  
  
// Usa for loop para ingresar detalles del proceso como hora de llegada y hora de irrumpir/desencadenarse (burst)
for(i=0; i<NOP; i++)  {  
  printf("\n Ingrese la hora de llegada e irrumpir del proceso: [%d]\n", i+1);  
  printf(" Arrival time is: \t");  // Acepta arrival time  
  scanf("%d", &at[i]);  
  printf(" \nBurst time is: \t"); // Acepta Burst time  
  scanf("%d", &bt[i]);  
  temp[i] = bt[i]; // guarda/almacena burst time array temporal 
}  
// acepta Time quantum  
printf("Ingrese Time Quantum para el proceso: \t");  
scanf("%d", &quant);  
// Muestra # de proceso, burst time, tiempo de respuesta y tiempo de espera
printf("\n # Proceso \t\t Burst Time \t\t T. Respuesta \t\t T. Espera ");  
for(sum=0, i = 0; y!=0; )  {  
  if(temp[i] <= quant && temp[i] > 0){  
      sum = sum + temp[i];  
      temp[i] = 0;  
      count=1;  
      }     
      else if(temp[i] > 0)  {  
          temp[i] = temp[i] - quant;  
          sum = sum + quant;    
      }  
      if(temp[i]==0 && count==1)  {  
          y--; 
          printf("\n# Proceso [%d] \t\t %d\t\t\t\t %d\t\t\t %d", i+1, bt[i], sum-at[i], sum-at[i]-bt[i]);  
          wt = wt+sum-at[i]-bt[i];  
          tat = tat+sum-at[i];  
          count =0;     
      }  
      if(i==NOP-1)  {  
          i=0;  
      }  
      else if(at[i+1]<=sum)  {  
          i++;  
      }  
      else  {  
          i=0;  
      }  
}  
// Representa el promedio de tiempo de espera y de respuesta  
avg_wt = wt * 1.0/NOP;  
avg_tat = tat * 1.0/NOP;  
printf("\n Promedio de tiempo de respuesta: \t%f", avg_wt);  
printf("\n Promedio de tiempo de espera: \t%f", avg_tat);  
getch();  
}  
