#include <stdint.h>
#include <stdlib.h>
#include "RM/rmMemory.h"
#include "VM/vm.h"

int VMcounter = 0;
VM* createVM(uint8_t* memoryPointer);

int main() {
    RM_Memory* rmMemory = malloc(sizeof(RM_Memory));
    initialize_RM_memory(rmMemory);

    VM* vm1 = createVM(rmMemory->userMemory);
    VM* vm2 = createVM(rmMemory->userMemory);
    VM* vm3 = createVM(rmMemory->userMemory);

    destroyVM(vm1);
    destroyVM(vm2);
    destroyVM(vm3);
    return 0;
}

VM* createVM(uint8_t* memoryPointer){
    VM_Memory* vmMemory = malloc(sizeof(VM_Memory));
    initializeVirtualMemory(vmMemory, memoryPointer+ VMcounter* (DATA_MEMORY + CODE_MEMORY + FREE_MEMORY) * PAGE_SIZE * WORD_SIZE);

    CPU* cpu = malloc(sizeof(CPU));
    initCPU(cpu, vmMemory); 

    VM* vm = malloc(sizeof(VM));
    initVM(vm, cpu, vmMemory);

    VMcounter++;  
    return vm;
}