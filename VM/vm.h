#pragma once

#include "vmCpu.h"

#include "vmMemory.h"

#include "../RM/rm.h"

#include<stdlib.h>
#include<stdbool.h>

typedef struct {
    int id;
    VM_CPU* cpu;
    VM_Memory* memory;
} VM;

void initVM(VM* vm, VM_CPU* cpu, VM_Memory* memory, int id);

void destroyVM(VM* vm);

void runVM(RM* rm, VM* vm);

bool allowedToRun(RM* rm, VM* vm);