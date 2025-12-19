#pragma once
#include "../defines.h"
#include "kernel.h"

typedef struct Kernel Kernel;
typedef struct VM VM;

typedef struct ProcessList {
    int type;
    Kernel* kernel;
    VM* items[MAX_PROCESESSES];
    int count;
} ProcessList;

void initList(ProcessList* list, int type, Kernel* kernel);
void addItem(ProcessList* list, VM* vm);