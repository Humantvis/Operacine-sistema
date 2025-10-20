#pragma once

#include "cpu.h"
#include "memory.h"

typedef struct {
    CPU* cpu;
    Memory* memory;
} VM;

int initVM(VM* vm, CPU* cpu, Memory* memory);

int destroyVM(VM* vm);

void runVM(VM* vm);