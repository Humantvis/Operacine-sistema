#include "rmCpu.h"

#include "rm.h"
#include "../VM/vm.h"

#define VM_RAM_SIZE REGISTERS + 2

void initRMCPU(RM_CPU* cpu) {
    for (int i = 0; i < REGISTERS; i++) {
        cpu->r[i] = 0;
    }

    cpu->VMCounter = 0;
    
    cpu->ic = 0;

    cpu->offset = 0;

    cpu->fr = 0;

    cpu->mode = SUPER;

    cpu->ptr = 0;

    cpu->pi = 0;

    cpu->si = 0;
    
    cpu->ti = 0;
}

void mountVM(RM* rm, VM* vm) {
    *(rm->cpu->ic) = rm->memory->supervizorMemory[vm->id * VM_RAM_SIZE * PAGE_SIZE * WORD_SIZE];
    
    rm->cpu->fr = rm->memory->supervizorMemory[vm->id * VM_RAM_SIZE * PAGE_SIZE * WORD_SIZE + WORD_SIZE - 1];
    
    for (int i = 0; i < REGISTERS; i++) {
        rm->cpu->r[i] = rm->memory->supervizorMemory[vm->id * VM_RAM_SIZE * PAGE_SIZE * WORD_SIZE + 4 + i];
    }

    rm->cpu->mountedVMID = vm->id;

    rm->cpu->offset = 0;
}

void mountNewVM(RM* rm, VM* vm) {
    for (int i = 0; i < REGISTERS; i++) {
        rm->cpu->r[i] = 0;
    }

    rm->cpu->ic = vm->memory->codeMemory;

    rm->cpu->offset = 0;

    rm->cpu->fr = 0;

    rm->cpu->mountedVMID = vm->id;
}