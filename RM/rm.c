#include "rm.h"

void initRM(RM* rm, RM_CPU* cpu, RM_Memory* memory, Channel_device* channelDevice, PagingDevice* pagingDevice) {
    rm->cpu = cpu;
    rm->memory = memory;
    rm->channelDevice = channelDevice;
    rm->pagingDevice = pagingDevice;
    rm->sharedSemaphore = 1;
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
void setKernel(RM* rm, Kernel* kernel) {
    rm->kernel = kernel;
}