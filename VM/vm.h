#pragma once

#include "vmCpu.h"

#include "vmMemory.h"

typedef struct {
    int id;
    VM_CPU* cpu;
    VM_Memory* memory;
} VM;

void initVM(VM* vm, VM_CPU* cpu, VM_Memory* memory, int id);

void destroyVM(VM* vm);

void runVM(VM* vm);