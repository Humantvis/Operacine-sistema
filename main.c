#include <stdint.h>
#include <stdlib.h>
#include "VM/vm.h"
#include "RM/rm.h"

int VMcounter = 0;
VM* createVM(RM* rm, int id);

int main() {
    RM_Memory* rmMemory = malloc(sizeof(RM_Memory));
    initialize_RM_memory(rmMemory);

    RM_CPU* rmCpu = malloc(sizeof(RM_CPU));
    initRMCPU(rmCpu);

    RM* rm = malloc(sizeof(RM));
    initRM(rm, rmCpu, rmMemory);

    VM* vm1 = createVM(rm, 0);
    VM* vm2 = createVM(rm, 1);
    VM* vm3 = createVM(rm, 2);

    destroyVM(vm1);
    destroyVM(vm2);
    destroyVM(vm3);
    return 0;
}

VM* createVM(RM* rm, int id) {
    VM_Memory* vmMemory = malloc(sizeof(VM_Memory));
    initializeVirtualMemory(vmMemory, rm->memory->userMemory + VMcounter* (DATA_MEMORY + CODE_MEMORY + FREE_MEMORY) * PAGE_SIZE * WORD_SIZE);

    VM_CPU* cpu = malloc(sizeof(VM_CPU));
    initVMCPU(cpu, vmMemory);

    VM* vm = malloc(sizeof(VM));
    initVM(vm, cpu, vmMemory, id);

    VMcounter++;  
    return vm;
}