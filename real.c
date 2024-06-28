/*
    Project: CPU Scheduling Simulation
    Description: Simulates CPU scheduling using Rate-Monotonic and Earliest-Deadline-First
                 algorithms to manage the execution order of processes.

    File: real.c
    Author: Bruno Ribeiro
    Contact: Bruno Ribeiro <brlla@cesar.school>
    Organization: CESAR School

    Creation Date: 03/28/2024
    Last Updated: 06/28/2024
    License: MIT License

    Notes: This module is part of a larger system intended to demonstrate advanced
           concepts in operating system design and CPU scheduling techniques.
*/

#include <stdio.h>
#include <string.h>

// Data Structures
typedef struct
{
    char name[100];
    int period;
    int CPUburst;

    int deadline;
    int remainingTime;

    int executionTime;

    int lost;
    int finished;
    int killed;
} Task;

void readFile(const char *inputFile, Task tasks[], int numProcesses);

#ifdef RATE_MONOTONIC
void rate(Task tasks[], int numProcesses, int timeLimit);
#endif

#ifdef EARLIEST_DEADLINE_FIRST
void edf(Task tasks[], int numProcesses, int timeLimit);
#endif

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *inputFile;
    char nameFile[100];
    strcpy(nameFile, argv[1]);

    inputFile = fopen(nameFile, "r");

    int timeLimit = 0;
    fscanf(inputFile, "%d", &timeLimit);

    int numProcesses = 0;
    Task process[1000];

    while (fscanf(inputFile, "%s %d %d", process[numProcesses].name, &process[numProcesses].period, &process[numProcesses].CPUburst) == 3)
    {
        process[numProcesses].deadline = process[numProcesses].period;
        process[numProcesses].remainingTime = process[numProcesses].CPUburst;
        numProcesses++;
    }

    fclose(inputFile);

#ifdef RATE_MONOTONIC
    rate(process, numProcesses, timeLimit);
#endif

#ifdef EARLIEST_DEADLINE_FIRST
    edf(process, numProcesses, timeLimit);
#endif

    return 0;
}

#ifdef RATE_MONOTONIC
void rate(Task tasks[], int numProcesses, int timeLimit)
{
    int time = 0;
    int idle = 0;
    int lastProcess = -1;

    FILE *outputFile;
    outputFile = fopen("rate.out", "w+");
    fprintf(outputFile, "EXECUTION BY RATE\n");

    while (time < timeLimit)
    {
        int minPeriod = 1000000000;
        int minIndex = -1;

        for (int i = 0; i < numProcesses; i++)
        {
            if ((minIndex == -1 || tasks[i].period < minPeriod) && tasks[i].remainingTime > 0)
            {
                minPeriod = tasks[i].period;
                minIndex = i;
            }
        }

        if (minIndex != -1)
        {
            if (minIndex != lastProcess)
            {
                if (idle > 0)
                {
                    fprintf(outputFile, "idle for %d units\n", idle);
                    idle = 0;
                }

                if (lastProcess != -1 && tasks[lastProcess].remainingTime > 0)
                {
                    if (tasks[lastProcess].executionTime > 0)
                    {
                        fprintf(outputFile, "[%s] for %d units - H\n", tasks[lastProcess].name, tasks[lastProcess].executionTime);
                    }
                    tasks[lastProcess].executionTime = 0;
                }
                lastProcess = minIndex;
            }

            tasks[minIndex].executionTime++;
            tasks[minIndex].remainingTime--;

            if (tasks[minIndex].remainingTime == 0)
            {
                if (tasks[minIndex].executionTime > 0)
                {
                    fprintf(outputFile, "[%s] for %d units - F\n", tasks[minIndex].name, tasks[minIndex].executionTime);
                }
                tasks[minIndex].finished++;
                tasks[minIndex].executionTime = 0;
            }
        }
        else
        {
            idle++;
        }

        for (int i = 0; i < numProcesses; i++)
        {
            tasks[i].deadline--;

            if (tasks[i].deadline == 0)
            {
                if (tasks[i].remainingTime > 0)
                {
                    if (tasks[i].executionTime > 0)
                    {
                        fprintf(outputFile, "[%s] for %d units - L\n", tasks[i].name, tasks[i].executionTime);
                    }
                    tasks[i].lost++;
                    tasks[i].executionTime = 0;
                }
                tasks[i].remainingTime = tasks[i].CPUburst;
                tasks[i].deadline = tasks[i].period;
                tasks[i].executionTime = 0;
            }
        }
        time++;
        lastProcess = minIndex;
    }

    if (idle > 0)
    {
        fprintf(outputFile, "idle for %d units\n", idle);
        idle = 0;
    }

    for (int i = 0; i < numProcesses; i++)
    {
        if (tasks[i].remainingTime > 0 || tasks[i].period == timeLimit)
        {
            if (tasks[i].executionTime > 0)
            {
                fprintf(outputFile, "[%s] for %d units - K\n", tasks[i].name, tasks[i].executionTime);
            }
            tasks[i].executionTime = 0;
            tasks[i].killed++;
        }
    }

    fprintf(outputFile, "\n");

    fprintf(outputFile, "LOST DEADLINES\n");
    for (int i = 0; i < numProcesses; i++)
    {
        fprintf(outputFile, "[%s] %d\n", tasks[i].name, tasks[i].lost);
    }

    fprintf(outputFile, "\n");

    fprintf(outputFile, "COMPLETE EXECUTION\n");
    for (int i = 0; i < numProcesses; i++)
    {
        fprintf(outputFile, "[%s] %d\n", tasks[i].name, tasks[i].finished);
    }

    fprintf(outputFile, "\n");

    fprintf(outputFile, "KILLED");
    for (int i = 0; i < numProcesses; i++)
    {
        fprintf(outputFile, "\n[%s] %d", tasks[i].name, tasks[i].killed);
    }

    fclose(outputFile);
}
#endif

#ifdef EARLIEST_DEADLINE_FIRST
void edf(Task tasks[], int numProcesses, int timeLimit)
{
    int time = 0;
    int idle = 0;
    int lastProcess = -1;

    FILE *outputFile;
    outputFile = fopen("edf.out", "w+");
    fprintf(outputFile, "EXECUTION BY EDF\n");

    while (time < timeLimit)
    {
        int closestDeadline = 1000000000;
        int minIndex = -1;

        for (int i = 0; i < numProcesses; i++)
        {
            if ((minIndex == -1 || tasks[i].deadline < closestDeadline) && tasks[i].remainingTime > 0)
            {
                closestDeadline = tasks[i].deadline;
                minIndex = i;
            }
        }

        if (minIndex != -1)
        {
            if (minIndex != lastProcess)
            {
                if (idle > 0)
                {
                    fprintf(outputFile, "idle for %d units\n", idle);
                    idle = 0;
                }

                if (lastProcess != -1 && tasks[lastProcess].remainingTime > 0)
                {
                    if (tasks[lastProcess].executionTime > 0)
                    {
                        fprintf(outputFile, "[%s] for %d units - H\n", tasks[lastProcess].name, tasks[lastProcess].executionTime);
                    }
                    tasks[lastProcess].executionTime = 0;
                }
                lastProcess = minIndex;
            }

            tasks[minIndex].executionTime++;
            tasks[minIndex].remainingTime--;

            if (tasks[minIndex].remainingTime == 0)
            {
                if (tasks[minIndex].executionTime > 0)
                {
                    fprintf(outputFile, "[%s] for %d units - F\n", tasks[minIndex].name, tasks[minIndex].executionTime);
                }
                tasks[minIndex].finished++;
                tasks[minIndex].executionTime = 0;
            }
        }
        else
        {
            idle++;
        }

        for (int i = 0; i < numProcesses; i++)
        {
            tasks[i].deadline--;

            if (tasks[i].deadline == 0)
            {
                if (tasks[i].remainingTime > 0)
                {
                    if (tasks[i].executionTime > 0)
                    {
                        fprintf(outputFile, "[%s] for %d units - L\n", tasks[i].name, tasks[i].executionTime);
                    }
                    tasks[i].lost++;
                    tasks[i].executionTime = 0;
                }
                tasks[i].remainingTime = tasks[i].CPUburst;
                tasks[i].deadline = tasks[i].period;
                tasks[i].executionTime = 0;
            }
        }
        time++;
        lastProcess = minIndex;
    }

    if (idle > 0)
    {
        fprintf(outputFile, "idle for %d units\n", idle);
        idle = 0;
    }

    for (int i = 0; i < numProcesses; i++)
    {
        if (tasks[i].remainingTime > 0 || tasks[i].period == timeLimit)
        {
            if (tasks[i].executionTime > 0)
            {
                fprintf(outputFile, "[%s] for %d units - K\n", tasks[i].name, tasks[i].executionTime);
            }
            tasks[i].executionTime = 0;
            tasks[i].killed++;
        }
    }

    fprintf(outputFile, "\n");

    fprintf(outputFile, "LOST DEADLINES\n");
    for (int i = 0; i < numProcesses; i++)
    {
        fprintf(outputFile, "[%s] %d\n", tasks[i].name, tasks[i].lost);
    }

    fprintf(outputFile, "\n");

    fprintf(outputFile, "COMPLETE EXECUTION\n");
    for (int i = 0; i < numProcesses; i++)
    {
        fprintf(outputFile, "[%s] %d\n", tasks[i].name, tasks[i].finished);
    }

    fprintf(outputFile, "\n");

    fprintf(outputFile, "KILLED");
    for (int i = 0; i < numProcesses; i++)
    {
        fprintf(outputFile, "\n[%s] %d", tasks[i].name, tasks[i].killed);
    }

    fclose(outputFile);
}
#endif
