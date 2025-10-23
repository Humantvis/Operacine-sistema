#pragma once

#include "rmCpu.h"
#include "rmMemory.h"
#include "channelDevice.h"

typedef struct RM {
    RM_CPU* cpu;
    RM_Memory* memory;
    Channel_device* channel;
} RM;

void initRM(RM* rm, RM_CPU* cpu, RM_Memory* memory);

void destroyRM(RM* rm);