#pragma once

#include "../RM/rm.h"

#include "vm.h"

#include "../defines.h"

typedef struct RM RM;
typedef struct VM VM;

typedef struct VM_CPU {
	uint8_t* r[REGISTERS];
    uint16_t* ic;
    int* offset;
    uint8_t* fr;
} VM_CPU;

void initVMCPU(VM_CPU* cpu, RM* rm);

void setRegister(VM* vm, int reg, uint8_t value);
uint8_t getRegister(VM* vm, int reg);

void setIc(VM* vm, uint16_t ic);
uint16_t getIc(VM* vm);

void setOffset(VM* vm, int offset);
int getOffset(VM* vm);

void setFr(VM* vm, uint8_t fr);
uint8_t getFr(VM* vm);