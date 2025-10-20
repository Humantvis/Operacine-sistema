#include "vm.h"

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

}