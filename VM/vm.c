#include "vm.h"

#define HALT 0

#include<stdlib.h>

void initVM(VM* vm, CPU* cpu, VM_Memory* memory) {
    vm->cpu = cpu;
    vm->memory = memory;
}

void destroyVM(VM* vm) {
    if (vm == NULL) {
        return;
    }

    free(vm->cpu);
    free(vm->memory);
    free(vm);
}

void runVM(VM* vm) {
    while (1) {
        uint8_t* instr_ptr = vm->memory->codeMemory + (*(vm->cpu->ic)) * WORD_SIZE;

        uint32_t instruction = *(uint32_t*)instr_ptr;

        if (instruction == HALT) // case when HALT - break
            break;

        (*(vm->cpu->ic))++;
    }
}