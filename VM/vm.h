#pragma once

#include "cpu.h"
#include "vmmemory.h"

typedef struct {
    CPU* cpu;
    VM_Memory* memory;
} VM;

void initVM(VM* vm, CPU* cpu, VM_Memory* memory);

void runVM(VM* vm);