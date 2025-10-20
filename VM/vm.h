#pragma once

#include "cpu.h"
#include "vmMemory.h"

typedef struct {
    CPU* cpu;
    VM_Memory* memory;
} VM;

void initVM(VM* vm, CPU* cpu, VM_Memory* memory);

void destroyVM(VM* vm);

void runVM(VM* vm);