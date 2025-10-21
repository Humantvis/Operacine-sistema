#pragma once

#include <stdint.h>

typedef struct RM RM;
typedef struct VM VM;

enum {
    SUPER,
    USER
};

#define REGISTERS 16

typedef struct {
    int mountedVMID;
	uint8_t r[REGISTERS];
    uint8_t* ic;
    uint8_t fr;
    uint8_t mode;
    uint8_t ptr;
    uint8_t pi;
    uint8_t si;
    uint8_t ti;
} RM_CPU;

void initRMCPU(RM_CPU* cpu);

void mountVM(RM* rm, VM* vm);