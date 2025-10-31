#include "vmMemory.h"
int initializeVirtualMemory (VM_Memory* vmMemory, uint8_t* datapointer) {
    vmMemory->dataMemory = datapointer;
    vmMemory->codeMemory = datapointer + DATA_MEMORY * PAGE_SIZE;
    vmMemory->freeMemory = datapointer + (DATA_MEMORY + CODE_MEMORY) * PAGE_SIZE;
    return 0; 
}