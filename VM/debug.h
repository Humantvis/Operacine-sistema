#pragma once
#include "../RM/rm.h"
#include "../VM/vm.h"
#include "../RM/channelDevice.h"

#include "../defines.h"

typedef struct RM RM;
typedef struct VM VM;
typedef struct Channel_device Channel_device;
typedef struct Kernel Kernel;

uint8_t debug(Kernel* kernel, VM* vm, Channel_device* channelDevice);