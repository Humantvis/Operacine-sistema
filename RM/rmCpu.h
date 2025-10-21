#pragma once

#include <stdint.h>

#include "vm.h"
#include "rm.h"

#define REGISTERS 16

typedef struct {
	uint8_t r[REGISTERS];
    uint8_t* ic;
    uint8_t fr;
} RM_CPU;

void initRMCPU(RM_CPU* cpu);

void mountVM(RM* rm, VM* vm);