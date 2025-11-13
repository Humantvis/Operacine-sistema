#include "rmCpu.h"

#include "rm.h"
#include "../VM/vm.h"

#define VM_RAM_SIZE (REGISTERS + sizeof(uintptr_t) + 2)

void initRMCPU(RM_CPU* cpu) {
    for (int i = 0; i < USER_MEMORY_SIZE / (DATA_MEMORY + CODE_MEMORY + FREE_MEMORY); i++) {
        cpu->VMs[i] = false;
    }

    cpu->mountedVMID = -1;

    for (int i = 0; i < REGISTERS; i++) {
        cpu->r[i] = 0;
    }

    cpu->ic = NULL;

    cpu->offset = -1;

    cpu->fr = 0;

    cpu->mode = SUPER;

    cpu->ptr = 0;

    cpu->pi = 0;

    cpu->si = 0;
    
    cpu->ti = 0;
    
}

void mountVM(RM* rm, int vmID) {
    for (int i = 0; i < REGISTERS; i++) {
        rm->cpu->r[i] = *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + i);
    }
    
    uintptr_t ic_addr;
    memcpy(&ic_addr,(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS), sizeof(uintptr_t));
    rm->cpu->ic = (uint8_t *)ic_addr;

    rm->cpu->offset = *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t));

    rm->cpu->fr = *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t) + 1);

    rm->cpu->mountedVMID = vmID;
}

void unmountVM(RM* rm) {
    int vmID = rm->cpu->mountedVMID;

    for (int i = 0; i < REGISTERS; i++) {
        *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + i) = rm->cpu->r[i];
    }

    uintptr_t ic_addr = (uintptr_t)rm->cpu->ic;
    memcpy((rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS), &ic_addr, sizeof(uintptr_t));

    *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t)) = rm->cpu->offset;

    *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t) + 1) = rm->cpu->fr;

    rm->cpu->mountedVMID = -1;
}

void addNewVM(RM* rm, int vmID) {
    for (int i = 0; i < REGISTERS; i++) {
        *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + i) = 0;
    }

     uint8_t *ic_start = rm->memory->userMemory + vmID * TOTAL_MEMORY_SIZE + DATA_MEMORY * PAGE_SIZE;

    uintptr_t ic_addr = (uintptr_t)ic_start;
    memcpy((rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS), &ic_addr, sizeof(uintptr_t));

    *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t)) = 0;

    *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t) + 1) = 0;

    rm->cpu->VMs[vmID] = true;
}

void removeVM(RM* rm, int vmID) {
    for (int i = 0; i < REGISTERS; i++) {
        *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + i) = 0;
    }

    for (size_t i = 0; i < sizeof(uintptr_t); i++) {
        *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + i) = 0;
    }

    *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t)) = 0;

    *(rm->memory->supervizorMemory + vmID * VM_RAM_SIZE + REGISTERS + sizeof(uintptr_t) + 1) = 0;

    rm->cpu->VMs[vmID] = false;
}