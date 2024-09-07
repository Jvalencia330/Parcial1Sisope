// Jesus Valencia 
// Ing. de sistemas - Parcial 1 Sistemas Operativos

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Tipo de estructura Process para almacenar todos los datos de un proceso
typedef struct {
    char id;
    int at;
    int bt;
    int q;
    int ct;
    int wt;
    int tat;
} Process;

void roundRobin(Process processes[], int size, int quantum, int *currentTime) {
    int remainingBurst[size];
    bool done;

    // Creamos un arreglo con los tiempos de ejecucion para irlos modificando segun la iteracion de los procesos
    for (int i = 0; i < size; i++) {
        remainingBurst[i] = processes[i].bt;
    }

    do {
        done = true;

        for (int i = 0; i < size; i++) {
            if (remainingBurst[i] > 0) {
                done = false; // Quedan procesos por ejecutar

                if (remainingBurst[i] > quantum) {
                    *currentTime += quantum;
                    remainingBurst[i] -= quantum;
                } else {
                    *currentTime += remainingBurst[i];
                    processes[i].wt = *currentTime - processes[i].bt - processes[i].at;
                    processes[i].ct = *currentTime;
                    processes[i].tat = processes[i].ct - processes[i].at;
                    remainingBurst[i] = 0;
                }
            }
        }
    } while (!done);
}

void fcfs(Process processes[], int size, int *currentTime) {
    for (int i = 0; i < size; i++) {
        *currentTime += processes[i].bt;
        processes[i].ct = *currentTime;
        processes[i].wt = processes[i].ct - processes[i].at - processes[i].bt;
        processes[i].tat = processes[i].ct - processes[i].at;
    }
}

void MLQ(Process processes[], int size, int cuantum) {
    int currentTime = 0;
    int promTATrr = 0;
    int promTATfcfs = 0;
    int promWTrr = 0;
    int promWTfcfs = 0;
    int completionTime[size];
    int waitingTime[size];

    // Creamos dos arreglos con la estructura de process para separar los procesos
    Process rrProces[size], fcfsProces[size];
    int rrCount = 0, fcfsCount = 0;

    // Separamos los procesos segun su orden en las colas, 1 para roundRobin y 2 para FCFS
    for (int i = 0; i < size; i++) {
        if (processes[i].q == 1) {
            rrProces[rrCount] = processes[i];
            rrCount++;
        } else if (processes[i].q == 2) {
            fcfsProces[fcfsCount] = processes[i];
            fcfsCount++;
        }
    }

    if (rrCount > 0) {
        roundRobin(rrProces, rrCount, cuantum, &currentTime);
    }
    if (fcfsCount > 0) {
        fcfs(fcfsProces, fcfsCount, &currentTime);
    }
    // Calculo de TAT promedio
    for (int i = 0; i < rrCount; i++) {
        promTATrr += rrProces[i].tat;
    }
    for (int i = 0; i < fcfsCount; i++) {
        promTATfcfs += fcfsProces[i].tat;
    }
    int promTAT = (promTATrr + promTATfcfs) / size;

    // Calculo de WT promedio
    for (int i = 0; i < rrCount; i++) {
        promWTrr += rrProces[i].wt;
    }
    for (int i = 0; i < fcfsCount; i++) {
        promWTfcfs += fcfsProces[i].wt;
    }
    int promWT = (promWTrr + promWTfcfs) / size;

    
    printf("ID\tAT\tBT\tQ\tCT\tWT\n");
    for (int i = 0; i < rrCount; i++) {
        printf("%c\t%d\t%d\t%d\t%d\t%d\n", rrProces[i].id, rrProces[i].at, rrProces[i].bt, rrProces[i].q, rrProces[i].ct, rrProces[i].wt);
    }
    for (int i = 0; i < fcfsCount; i++) {
        printf("%c\t%d\t%d\t%d\t%d\t%d\n", fcfsProces[i].id, fcfsProces[i].at, fcfsProces[i].bt, fcfsProces[i].q, fcfsProces[i].ct, fcfsProces[i].wt);
    }
    printf("TAT Promedio = %d\n", promTAT);
    printf("WT Promedio = %d", promWT);

}

int main() {
    FILE *file;
    file = fopen("testcase1.txt", "r");

    int num_cases;
    fscanf(file, "%d", &num_cases); // Lee el numero de casos de prueba

    for (int i = 0; i < num_cases; i++) {
        int num_processes;
        int quantumRR;
        fscanf(file, "%d", &num_processes); // Lee el numero de procesos en el caso de prueba
        fscanf(file, "%d", &quantumRR);
        Process processes[num_processes];

        for (int j = 0; j < num_processes; j++) {
            // Lectura de los datos
            fscanf(file, " %c", &processes[j].id);
            fscanf(file, "%d", &processes[j].at);
            fscanf(file, "%d", &processes[j].bt);
            fscanf(file, "%d", &processes[j].q); 
        }
        // llamado a la funcion principal MLQ
        MLQ(processes,num_processes,quantumRR);
    }
    fclose(file);
    return 0;
}
