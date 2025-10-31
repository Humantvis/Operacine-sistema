#include <stdint.h>
#include <stdlib.h>
#include "VM/vm.h"
#include "RM/rm.h"
#include "parser/parser.h"
#include "VM/debug.h"
#include "RM/channelDevice.h"

VM* createVM(RM* rm, int id);
int parse(RM* rm, const char* filename, int offset);

int main() {
    RM_Memory* rmMemory = malloc(sizeof(RM_Memory));
    initialize_RM_memory(rmMemory);

    RM_CPU* rmCpu = malloc(sizeof(RM_CPU));
    initRMCPU(rmCpu);
    
    Channel_device* channel = malloc(sizeof(Channel_device));
    initChannelDevice(channel, rmCpu, rmMemory);

    RM* rm = malloc(sizeof(RM));
    initRM(rm, rmCpu, rmMemory, channel);

    if(parse(rm, "code.txt", 0) == -1) {
        destroyRM(rm);
        return -1;
    }

    VM* vm = createVM(rm, 0);

    mountVM(rm, 0);

    debug(rm, vm, channel);

    runVM(rm, vm);
    
    unmountVM(rm);

    destroyVM(vm);
    
    destroyRM(rm);
    return 0;
}

VM* createVM(RM* rm, int id) {
    if(rm->cpu->VMs[id]) {
        return NULL;
    }
    VM_Memory* vmMemory = malloc(sizeof(VM_Memory));
    initializeVirtualMemory(vmMemory, rm->memory->userMemory + id * TOTAL_MEMORY_SIZE);

    VM_CPU* cpu = malloc(sizeof(VM_CPU));
    initVMCPU(cpu, rm);

    VM* vm = malloc(sizeof(VM));
    initVM(rm, vm, cpu, vmMemory, id);

    addNewVM(rm, id);

    return vm;
}