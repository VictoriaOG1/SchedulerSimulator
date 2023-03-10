#include <stdio.h>

#define MAX_PROCESSES 100
#define TIME_QUANTUM 4

struct process {
    int pid;          // process ID
    int arrival_time; // arrival time
    int burst_time;   // burst time
    int remaining_time; // remaining time to complete execution
};

int main() {
    struct process processes[MAX_PROCESSES];
    int n, time = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;

    // Read input
    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Process %d\n", i + 1);
        printf("Enter arrival time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time: ");
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].pid = i + 1;
        printf("\n");
    }

    // Run the scheduler
    int remaining_processes = n;
    while (remaining_processes > 0) {
        // Process all arriving processes
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == time) {
                printf("Process %d arrives at time %d\n", processes[i].pid, time);
            }
        }

        // Choose the next process to execute
        int current_process = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= time) {
                if (current_process == -1 || processes[i].remaining_time < processes[current_process].remaining_time) {
                    current_process = i;
                }
            }
        }

        if (current_process == -1) {
            // No process available to run
            printf("CPU idle at time %d\n", time);
            time++;
            continue;
        }

        // Execute the process
        printf("Process %d starts execution at time %d\n", processes[current_process].pid, time);
        int time_slice = TIME_QUANTUM;
        if (processes[current_process].remaining_time < TIME_QUANTUM) {
            time_slice = processes[current_process].remaining_time;
        }
        processes[current_process].remaining_time -= time_slice;
        time += time_slice;

        // Process finishes
        if (processes[current_process].remaining_time == 0) {
            printf("Process %d finishes execution at time %d\n", processes[current_process].pid, time);
            int waiting_time = time - processes[current_process].burst_time - processes[current_process].arrival_time;
            int turnaround_time = time - processes[current_process].arrival_time;
            total_waiting_time += waiting_time;
            total_turnaround_time += turnaround_time;
            remaining_processes--;
        } else {
            // Process is preempted
            printf("Process %d is preempted at time %d\n", processes[current_process].pid, time);
        }
    }

    // Print performance metrics
    printf("Average waiting time: %.2f\n", total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", total_turnaround_time / n);

    return 0;
}