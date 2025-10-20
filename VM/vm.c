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
    while(vm->memory->codeMemory + (*(vm->cpu->ic) * WORD_SIZE) != HALT) {
        int currentWord = vm->memory->codeMemory + (*(vm->cpu->ic) * WORD_SIZE);

        //use current word to know what command to execute

        vm->cpu->ic++;
    }
}