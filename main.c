#include <stdint.h>
#include <stdlib.h>
#include "VM/vm.h"
#include "RM/rm.h"
#include "parser/parser.h"

VM* createVM(RM* rm, int id);
int parse(RM* rm, const char* filename, int offset);

int main() {
    RM_Memory* rmMemory = malloc(sizeof(RM_Memory));
    initialize_RM_memory(rmMemory);

    RM_CPU* rmCpu = malloc(sizeof(RM_CPU));
    initRMCPU(rmCpu);

    RM* rm = malloc(sizeof(RM));
    initRM(rm, rmCpu, rmMemory);

    if(parse(rm, "code.txt", 0) == -1) {
        destroyRM(rm);
        return -1;
    }

    VM* vm = createVM(rm, 0);

    mountVM(rm, 0);

<<<<<<< Updated upstream
=======
    //debug(rm, vm, channel);

>>>>>>> Stashed changes
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