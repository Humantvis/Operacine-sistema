#include "vm.h"

int initVM(VM* vm, CPU* cpu, Memory* memory) {
    vm->cpu = cpu;
    vm->memory = memory;
}

int destroyVM(VM* vm) {
    delete(vm->cpu);
    delete(vm->memory);
}

void runVM(VM* vm) {

}