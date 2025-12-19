#include "RM/rm.h"
#include "KERNEL/kernel.h"
#include "VM/vm.h"
#include "RM/externalMemory.h"
#include "parser/parser.h"
#include "VM/debug.h"

int main() {
    
    RM_CPU* rm_cpu = malloc(sizeof(RM_CPU));
    initRMCPU(rm_cpu);
    
    SupervisorMemory* supervisorMemory = malloc(sizeof(SupervisorMemory));
    initialize_RM_supervisorMemory(supervisorMemory);
    
    RM_Memory* memory = malloc(sizeof(RM_Memory));
    initialize_RM_memory(memory, supervisorMemory);
    
    Channel_device* channelDevice = malloc(sizeof(Channel_device));
    initChannelDevice(channelDevice, rm_cpu, memory);
    
    PagingDevice* pagingDevice = malloc(sizeof(PagingDevice));
    initPagingDevice(pagingDevice, supervisorMemory);

    RM* rm = malloc(sizeof(RM));
    initRM(rm, rm_cpu, memory, channelDevice, pagingDevice);

    ExternalMemory* externalMemory = malloc(sizeof(ExternalMemory));
    initializeExternalmemory(externalMemory);

    parse(externalMemory, "code.txt", 0);

    Kernel* kernel = malloc(sizeof(Kernel));
    initKernel(kernel, rm);

    scheduler(kernel);

    destroyRM(rm);
    return 0;
}