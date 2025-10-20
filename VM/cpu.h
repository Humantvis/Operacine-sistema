#pragma once

#include <stdint.h>

#include "vmmemory.h"

#define REGISTERS 16

typedef struct {
	uint8_t r[REGISTERS];
    uint8_t* ic;
    uint8_t fr;
} CPU;

void initCPU(CPU* cpu, VM_Memory* memory);