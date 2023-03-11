#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int burst_time;
    int arrival_time;
    int waiting_time;
};

struct Queue {
    struct Process** items;
    int front;
    int rear;
    int size;
    int capacity;
};

struct Process* newProcess(int pid, int burst_time, int arrival_time) {
    struct Process* process = (struct Process*) malloc(sizeof(struct Process));
    process->pid = pid;
    process->burst_time = burst_time;
    process->arrival_time = arrival_time;
    process->waiting_time = 0;
    return process;
}

struct Queue* newQueue(int capacity) {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->items = (struct Process**) malloc(capacity * sizeof(struct Process*));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

void enqueue(struct Queue* queue, struct Process* process) {
    if (queue->size == queue->capacity) {
        printf("Queue is full, cannot enqueue process\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->items[queue->rear] = process;
    queue->size++;
}

struct Process* dequeue(struct Queue* queue) {
    if (queue->size == 0) {
        return NULL;
    }
    struct Process* process = queue->items[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return process;
}

void simulateRR(int n, int quantum, struct Process** processes) {
    // Create a queue of processes
    struct Queue* queue = newQueue(n);
    for (int i = 0; i < n; i++) {
        enqueue(queue, processes[i]);
    }

    // Simulate round robin scheduling algorithm
    int time = 0;
    while (queue->size > 0) {
        struct Process* process = dequeue(queue);
        if (process->burst_time > quantum) {
            process->burst_time -= quantum;
            time += quantum;
            while (queue->rear != (queue->front - 1 + queue->capacity) % queue->capacity &&
                   queue->items[queue->rear]->arrival_time <= time) {
                enqueue(queue, dequeue(queue));
            }
            enqueue(queue, process);
        } else if (process->burst_time > 0) {
            time += process->burst_time;
            process->waiting_time = time - process->burst_time - process->arrival_time;
        }
    }

    // Calculate total waiting time and turnaround time
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i]->waiting_time;
        total_turnaround_time += processes[i]->waiting_time + processes[i]->burst_time;
    }

    // Print results
    printf("Round Robin Scheduling Algorithm Simulation\n");
    printf("==========================================\n");
    printf("Quantum: %d\n", quantum);
    printf("Processes:\n");
    printf("PID\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n",
        processes[i]->pid,
        processes[i]->burst_time,
        processes[i]->arrival_time,
        processes[i]->waiting_time,
        processes[i]->waiting_time + processes[i]->burst_time
        );
    }

    // Print average waiting time and turnaround time
    printf("==========================================\n");
    printf("Average waiting time: %.2f\n", (float) total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) total_turnaround_time / n);
}

int main() {
    // Get number of processes from user
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Get quantum time from user
    int quantum;
    printf("Enter quantum time: ");
    scanf("%d", &quantum);

    // Get process data from user
    struct Process** processes = (struct Process**) malloc(n * sizeof(struct Process*));
    for (int i = 0; i < n; i++) {
        int pid, burst_time, arrival_time;
        printf("Enter data for process %d:\n", i+1);
        printf("PID: ");
        scanf("%d", &pid);
        printf("Burst Time: ");
        scanf("%d", &burst_time);
        printf("Arrival Time: ");
        scanf("%d", &arrival_time);
        processes[i] = newProcess(pid, burst_time, arrival_time);
    }

    // Simulate round robin scheduling algorithm
    simulateRR(n, quantum, processes);

    // Free memory used by processes and queue
    for (int i = 0; i < n; i++) {
        free(processes[i]);
    }
    free(processes);

    return 0;
}