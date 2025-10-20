#include "vm.h"

#define HALT 0

void initVM(VM* vm, CPU* cpu, VM_Memory* memory) {
    vm->cpu = cpu;
    vm->memory = memory;
}

void runVM(VM* vm) {
    while(vm->memory->codeMemory[vm->cpu->ic] != HALT) {
        
        (vm->cpu->ic) += WORD_SIZE;
    }
}