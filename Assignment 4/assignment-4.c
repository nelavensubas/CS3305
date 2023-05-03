/*
 * Assignment 4
 * Author: Nelaven Subaskaran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1024

struct Process {
    char processName[1024];
    int burstTime;
    int arrivalTime;
    int waitingTime;
    int turnAroundTime;
    int arrived;
    int processExecuted;
};

int main(int argc, char *argv[])
{
    if(argc != 3 && argc != 4) {
        printf("Proper usage is ./assignment-4 [-f|-s|-r <quantum>] <Input file name>\n");
        exit(1);
    }

    // Figure out the number of processes
    FILE* fp = fopen("assignment-4-input.csv", "r");
    if (fp == NULL) {
        printf("Could not open %s\n", (argc == 3) ? argv[2] : argv[3]);
        exit(1);
    }
    int numLines = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            numLines++;
        }
    }
    struct Process processes[numLines];
    fclose(fp);
    
    // Store the names and burst times for all processes
    fp = fopen("assignment-4-input.csv", "r");
    char line[MAX_LINE_LENGTH];
    char *token;
    int i = 0;
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        token = strtok(line, ",");
        strncpy(processes[i].processName, token, 1024);

        token = strtok(NULL, ",");
        processes[i].burstTime = atoi(token);

        processes[i].arrivalTime = i;
        processes[i].waitingTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].arrived = -1;
        processes[i].processExecuted = 0;

        i++;
    }
    fclose(fp);

    int ticks = 0;

    if (argc == 3)
    {
        // First Come First Served Scheduling Algorithm
        if (strcmp(argv[1], "-f") == 0) {
            printf("First Come First Served\n");
            int waitTime = 0, newWaitTime = 0;
            int turnAroundTime = 0;
            for(int i = 0; i < numLines; i++) {
                int currentBurstTime = processes[i].burstTime;
                while (currentBurstTime > 0) {
                    printf("T%d : P%d  - Burst left  %d, Wait time  %d, Turnaround time  %d\n", ticks, i, currentBurstTime, waitTime, turnAroundTime);
                    ticks++;
                    currentBurstTime--;
                    if(ticks >= i && currentBurstTime != 0) {
                        newWaitTime++;
                        turnAroundTime++;
                    }
                }
                processes[i].waitingTime = waitTime;
                processes[i].turnAroundTime = turnAroundTime;
                waitTime = newWaitTime;
            }

            float totalAverageWaitingTime = 0;
            float totalTurnAroundTime = 0;
            for(int i = 0; i < numLines; i++) {
                totalAverageWaitingTime += processes[i].waitingTime;
                totalTurnAroundTime += processes[i].turnAroundTime;
            }

            totalAverageWaitingTime = (float)totalAverageWaitingTime / (float)numLines;
            totalTurnAroundTime = (float)totalTurnAroundTime / (float)numLines;

            printf("\nTotal average waiting time:     %0.1f\n", totalAverageWaitingTime);
            printf("Total average turnaround time:  %0.1f\n", totalTurnAroundTime);
        } else if (strcmp(argv[1], "-s") == 0) {
            // Shortest Job First Scheduling Algorithm
            printf("Shortest Job First\n");
            int doneProcess = 0;
            int currentProcess = 0;
            int prevProcess = 0;

            while(doneProcess < numLines) {
                for(int i = 0; i <= ticks; i++) {
                    if (i >= numLines) {
                        break;
                    }
                    if (processes[currentProcess].burstTime == 0 || (processes[i].burstTime > 0 && processes[i].burstTime <= processes[currentProcess].burstTime)) {
                        currentProcess = i;
                    }
                    if (processes[i].arrived != -1) {
                        processes[i].turnAroundTime++;
                    }
                }

                if (prevProcess != currentProcess) {
                    prevProcess = currentProcess;
                    processes[currentProcess].waitingTime = ticks - processes[currentProcess].processExecuted - processes[currentProcess].arrivalTime;
                }

                if (processes[currentProcess].arrived == -1)
                {
                    processes[currentProcess].arrived = 0;
                    processes[currentProcess].turnAroundTime = ticks - processes[currentProcess].arrivalTime;
                }
                printf("T%d : %s  - Burst left  %d, Wait time  %d, Turnaround time  %d\n", ticks, processes[currentProcess].processName, processes[currentProcess].burstTime, processes[currentProcess].waitingTime, processes[currentProcess].turnAroundTime);

                processes[currentProcess].burstTime--;
                processes[currentProcess].processExecuted++;

                if (processes[currentProcess].burstTime == 0) {
                    doneProcess++;
                }

                ticks++;
            }

            float totalAverageWaitingTime = 0;
            float totalTurnAroundTime = 0;
            for (int i = 0; i < numLines; i++)
            {
                totalAverageWaitingTime += processes[i].waitingTime;
                totalTurnAroundTime += processes[i].turnAroundTime;
            }

            totalAverageWaitingTime = (float)totalAverageWaitingTime / (float)numLines;
            totalTurnAroundTime = (float)totalTurnAroundTime / (float)numLines;

            printf("\nTotal average waiting time:     %0.1f\n", totalAverageWaitingTime);
            printf("Total average turnaround time:  %0.1f\n", totalTurnAroundTime);
        }
    } else if (argc == 4) {
        // Round Robin Scheduling Algorithm
        printf("Round Robin with Quantum %s\n", argv[2]);

        int quantumTime = atoi(argv[2]);
        int runningTime = 0;

        int doneProcesses = 0;

        int prevProcess = 0;
        int currentProcess = 0;

        int turnAroundTime = -1;
        while (doneProcesses < numLines) {
            if (currentProcess >= numLines || processes[currentProcess].burstTime <= 0) {
                for(int x = 0; x < numLines; x++) {
                    if(processes[x].burstTime > 0) {
                        currentProcess = x;
                        break;
                    }
                }
            }

            if (prevProcess != currentProcess) {
                processes[currentProcess].waitingTime = ticks - processes[currentProcess].processExecuted - processes[currentProcess].arrivalTime;
                prevProcess = currentProcess;
                processes[currentProcess].turnAroundTime = turnAroundTime;
            } else {
                turnAroundTime++;
                processes[currentProcess].turnAroundTime = turnAroundTime;
            }

            printf("T%d : %s  - Burst left  %d, Wait time  %d, Turnaround time  %d\n", ticks, processes[currentProcess].processName,
                processes[currentProcess].burstTime, processes[currentProcess].waitingTime, processes[currentProcess].turnAroundTime);
            
            processes[currentProcess].burstTime--;
            processes[currentProcess].processExecuted++;
            runningTime++;

            if (runningTime == quantumTime) {
                currentProcess++;
                runningTime = 0;
            }

            if (processes[currentProcess].burstTime <= 0) {
                currentProcess++;
                runningTime = 0;
                doneProcesses++;
            }

            ticks++;
        }

        float totalAverageWaitingTime = 0;
        float totalTurnAroundTime = 0;
        for (int i = 0; i < numLines; i++) {
            totalAverageWaitingTime += processes[i].waitingTime;
            totalTurnAroundTime += processes[i].turnAroundTime;
        }

        totalAverageWaitingTime = (float)totalAverageWaitingTime / (float)numLines;
        totalTurnAroundTime = (float)totalTurnAroundTime / (float)numLines;

        printf("\nTotal average waiting time:     %0.1f\n", totalAverageWaitingTime);
        printf("Total average turnaround time:  %0.1f\n", totalTurnAroundTime);
    }

    return 0;
}