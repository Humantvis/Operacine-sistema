#include "rm.h"

#include<stdlib.h>

void initRM(RM* rm, RM_CPU* cpu, RM_Memory* memory, Channel_device* channelDevice) {
    rm->cpu = cpu;
    rm->memory = memory;
    rm->channelDevice = channelDevice;
}

void destroyRM(RM* rm) {
    if (rm == NULL) {
        return;
    }

    free_RM_memory(rm->memory);

    free(rm->cpu);
    free(rm->memory);
    free(rm);
}