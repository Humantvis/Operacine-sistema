#pragma once

#include <stdint.h>

#include "../RM/rm.h"
#include "vm.h"

#define REGISTERS 16

typedef struct VM_CPU {
	uint8_t* r[REGISTERS];
    uint8_t** ic;
    int* offset;
    uint8_t* fr;
} VM_CPU;

void initVMCPU(VM_CPU* cpu, RM* rm);

void setRegister(VM* vm, int reg, uint8_t value);
uint8_t getRegister(VM* vm, int reg);

void setIc(VM* vm, uint8_t* ic);
uint8_t* getIc(VM* vm);

void setOffset(VM* vm, int offset);
int getOffset(VM* vm);

void setFr(VM* vm, uint8_t fr);
uint8_t getFr(VM* vm);