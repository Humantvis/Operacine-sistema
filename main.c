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

    parse(rm, "code.txt", 0);

    VM* vm = createVM(rm, 0);

    runVM(rm, vm);
    
    destroyRM(rm);
    return 0;
}

VM* createVM(RM* rm, int id) {
    VM_Memory* vmMemory = malloc(sizeof(VM_Memory));
    initializeVirtualMemory(vmMemory, rm->memory->userMemory + rm->cpu->VMCounter * TOTAL_MEMORY_SIZE);

    VM_CPU* cpu = malloc(sizeof(VM_CPU));
    initVMCPU(cpu, vmMemory);

    VM* vm = malloc(sizeof(VM));
    initVM(vm, cpu, vmMemory, id);

    mountNewVM(rm, vm);

    rm->cpu->VMCounter++;

    return vm;
}