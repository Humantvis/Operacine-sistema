#pragma once
#include "../defines.h"
#include "kernel.h"

typedef struct ProcessList ProcessList;
typedef struct VM VM;

typedef struct Process {
    int id;
    int priority;
    int state;
    ProcessList* currentList;
    Kernel* kernel;
    VM* vm;
}Process;

void initProcess(Process* process, int id, int priority, Kernel* kernel, VM* vm);
void startProcess(Process* process, int priority, int id);