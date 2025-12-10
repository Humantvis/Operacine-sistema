#pragma once

#include "rmCpu.h"
#include "rmMemory.h"
#include "channelDevice.h"
#include "pagingDevice.h"

#include "../VM/vm.h"

#include "../defines.h"

typedef struct RM_CPU RM_CPU;
typedef struct RM_Memory RM_Memory;
typedef struct Channel_device Channel_device;

typedef struct RM {
    RM_CPU* cpu;
    RM_Memory* memory;
    Channel_device* channelDevice;
    PagingDevice* pagingDevice;
} RM;

void initRM(RM* rm, RM_CPU* cpu, RM_Memory* memory, Channel_device* channelDevice, PagingDevice* pagingDevice);

void destroyRM(RM* rm);