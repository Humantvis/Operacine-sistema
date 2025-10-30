#pragma once

#include "rmCpu.h"
#include "rmMemory.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
int inputchannel(Channel_device* channelDevice);
int outputchannel(Channel_device* channelDevice, const char *data);

enum Objects {
    User_Memory,
    Supervizor_Memory,
    Shared_Memory,
    IO_Device,
    CPU
};

