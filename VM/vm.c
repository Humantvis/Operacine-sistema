#include "vm.h"

#define HALT 0

#include<stdlib.h>

void initVM(VM* vm, VM_CPU* cpu, VM_Memory* memory, int id) {
    vm->cpu = cpu;
    vm->memory = memory;
    vm->id = id;
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
    while (allowedToRun(vm)) {
        uint8_t* instr_ptr = vm->memory->codeMemory + (*(vm->cpu->ic)) * WORD_SIZE;

        uint32_t instruction = *(uint32_t*)instr_ptr;

        if (instruction == HALT) // case when HALT - break
            break;

        (*(vm->cpu->ic))++;
    }
}