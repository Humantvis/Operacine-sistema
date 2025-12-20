#pragma once
#include "../defines.h"
#include "kernel.h"

typedef struct Kernel Kernel;
typedef struct VM VM;
typedef struct Process Process;

typedef struct ProcessList {
    int type;
    Kernel* kernel;
    Process* items[MAX_PROCESESSES];
    int count;
} ProcessList;

void initList(ProcessList* list, int type, Kernel* kernel);
void addItem(ProcessList* list, Process* process);
void jobToSwap(Process* process);