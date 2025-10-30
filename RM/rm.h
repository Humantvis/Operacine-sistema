#pragma once

#include "rmCpu.h"
#include "rmMemory.h"
#include "channelDevice.h"

typedef struct RM {
    RM_CPU* cpu;
    RM_Memory* memory;
    Channel_device* channelDevice;
} RM;

void initRM(RM* rm, RM_CPU* cpu, RM_Memory* memory, Channel_device* channelDevice);

void destroyRM(RM* rm);