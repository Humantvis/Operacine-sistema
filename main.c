#include <stdint.h>
#include <stdlib.h>
#include "RM/rmMemory.h"
#include "VM/vm.h"

int VMcounter = 0;
VM* createVM(uint8_t* memoryPointer);

int main() {
    
    RM_Memory* rmMemory = malloc(sizeof(RM_Memory));
    initialize_RM_memory(rmMemory);

    createVM(rmMemory->userMemory);
    createVM(rmMemory->userMemory);
    createVM(rmMemory->userMemory);
    return 0;
}
VM* createVM(uint8_t* memoryPointer){
    VM_Memory* vmMemory;
    initializeVirtualMemory(vmMemory, memoryPointer+ VMcounter* (DATA_MEMORY + CODE_MEMORY + FREE_MEMORY) * PAGE_SIZE * WORD_SIZE);

    CPU* cpu;
    initCPU(cpu, vmMemory); 

    VM* vm;
    initVM(vm, cpu, vmMemory);

    VMcounter++;  
    return vm;
}