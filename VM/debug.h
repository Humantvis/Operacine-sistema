#pragma once
#include "../RM/rm.h"
#include "../VM/vm.h"
#include "../RM/channelDevice.h"
#include <stdint.h>
#include <stdio.h>


void debug(RM* rm, VM* vm, Channel_device* channelDevice);

enum Actions {
    RUN_FULL,
    STEP,
    REGISTER_VALUES,
    VM_MEM
};