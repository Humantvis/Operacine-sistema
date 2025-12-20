#include "RM/rm.h"
#include "KERNEL/kernel.h"
#include "VM/vm.h"
#include "RM/externalMemory.h"
#include "parser/parser.h"
#include "VM/debug.h"

int main() {
    ExternalMemory* externalMemory = malloc(sizeof(ExternalMemory));
    initializeExternalmemory(externalMemory);

    //visos programos cia turi buti parsed
    /*for(int i = 0; i < 1; i++) {
        char*  fileName;
        snprintf(fileName, sizeof(fileName), "%d.txt", i);
        parse(externalMemory, fileName, 0);
    }*/

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

    Kernel* kernel = malloc(sizeof(Kernel));
    initKernel(kernel, rm, externalMemory);

    setKernel(rm, kernel);
    start(kernel);

    


    destroyRM(rm);
    return 0;
}