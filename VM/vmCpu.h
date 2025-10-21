#pragma once

#include <stdint.h>

#include "vmmemory.h"

#define REGISTERS 16

typedef struct {
	uint8_t r[REGISTERS];
    uint8_t* ic;
    uint8_t fr;
} VM_CPU;

void initVMCPU(VM_CPU* cpu, VM_Memory* memory);