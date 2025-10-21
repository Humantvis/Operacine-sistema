#pragma once

#include "rmCpu.h"
#include "rmMemory.h"

typedef struct RM {
    RM_CPU* cpu;
    RM_Memory* memory;
} RM;

void initRM(RM* rm, RM_CPU* cpu, RM_Memory* memory);

void destroyRM(RM* rm);