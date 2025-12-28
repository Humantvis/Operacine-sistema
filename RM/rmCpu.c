#include "rmCpu.h"

#define VM_RAM_SIZE (REGISTERS + sizeof(uintptr_t) + 2)

void initRMCPU(RM_CPU* cpu) {
    for (int i = 0; i < REGISTERS; i++) {
        cpu->r[i] = 0;
    }

    cpu->ic = 0;

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
        rm->cpu->r[i] = rm->memory->supervisorMemory->stateR[vmID][i];
    }

    rm->cpu->ic = rm->memory->supervisorMemory->stateCi[vmID];
    rm->cpu->offset = rm->memory->supervisorMemory->stateOffset[vmID];
    rm->cpu->fr = rm->memory->supervisorMemory->stateFr[vmID];

    rm->memory->supervisorMemory->mountedVMID = vmID;
}

void unmountVM(RM* rm) {
    int vmID = rm->memory->supervisorMemory->mountedVMID;

    for (int i = 0; i < REGISTERS; i++) {
        rm->memory->supervisorMemory->stateR[vmID][i] = rm->cpu->r[i];
    }

    rm->memory->supervisorMemory->stateCi[vmID] = rm->cpu->ic;
    rm->memory->supervisorMemory->stateOffset[vmID] = rm->cpu->offset;
    rm->memory->supervisorMemory->stateFr[vmID] = rm->cpu->fr;

    rm->memory->supervisorMemory->mountedVMID = -1;
}

void addNewVM(RM* rm, int vmID) {
    for (int i = 0; i < REGISTERS; i++) {
        rm->memory->supervisorMemory->stateR[vmID][i] = 0;
    }

    rm->memory->supervisorMemory->stateCi[vmID] = DATA_MEMORY * PAGE_SIZE;
    rm->memory->supervisorMemory->stateOffset[vmID] = 0;
    rm->memory->supervisorMemory->stateFr[vmID] = 0;
    rm->memory->supervisorMemory->VMs[vmID] = true;
}

void removeVM(RM* rm, int vmID) {
    for (int i = 0; i < REGISTERS; i++) {
        rm->memory->supervisorMemory->stateR[vmID][i] = 0;
    }

    rm->memory->supervisorMemory->stateCi[vmID] = 0;
    rm->memory->supervisorMemory->stateOffset[vmID] = 0;
    rm->memory->supervisorMemory->stateFr[vmID] = 0;
    rm->memory->supervisorMemory->VMs[vmID] = false;
}