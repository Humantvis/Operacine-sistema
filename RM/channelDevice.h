#pragma once

#include "rmCpu.h"
#include "rmMemory.h"

typedef struct Channel_device {
    uint8_t sb; 
    uint8_t db;
    uint8_t st;
    uint8_t dt;

    RM_Memory* memory;
    RM_CPU* cpu;
} Channel_device;

void initChannelDevice(Channel_device* channelDevice, RM_CPU* cpu, RM_Memory* memory);
void resetChannelDevice(Channel_device* channelDevice);
int callChannelDevice(Channel_device* channelDevice);

enum Objects {
    User_Memory,
    Supervizor_Memory,
    Shared_Memory,
    IO_Device,
    CPU
};

