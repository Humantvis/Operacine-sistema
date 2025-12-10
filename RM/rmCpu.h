#pragma once

#include "rm.h"
#include "../VM/vm.h"

#include "../defines.h"

#define BUFFER_SIZE 64

typedef struct RM RM;

typedef struct RM_CPU {
	uint8_t r[REGISTERS];
    uint16_t ic;
    int offset;
    uint8_t fr;
    uint8_t mode;
    uint8_t ptr;
    uint8_t pi;
    uint8_t si;
    uint8_t ti;
} RM_CPU;

void initRMCPU(RM_CPU* cpu);

void mountVM(RM* rm, int vmID);

void unmountVM(RM* rm);

void addNewVM(RM* rm, int vmID);

void removeVM(RM* rm, int vmID);