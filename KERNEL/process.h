#pragma once
#include "../defines.h"
#include "kernel.h"

typedef struct ProcessList ProcessList;
typedef struct VM VM;

typedef struct Process {
    int id;
    int priority;
    int state;
    int timeLeft;
    ProcessList* currentList;
    Kernel* kernel;
    VM* vm;

    bool debugInitialized;
    int lastDebugMode;
} Process;


void initProcess(Process* process, int id, int priority, Kernel* kernel, VM* vm);

void startProcess(ProcessList* processList, Process* process, int priority, int id);

void changeState(Process* process, int newState);

void preemptProcess(Kernel* kernel, Process* p);

void finishProcess(Kernel* kernel, Process* p);

Process* popProcess(ProcessList* list);