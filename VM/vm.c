#include "vm.h"

void initVM(VM* vm, CPU* cpu, VM_Memory* memory) {
    vm->cpu = cpu;
    vm->memory = memory;
}

void destroyVM(VM* vm) {
    //delete(vm->cpu);
    //delete(vm->memory);
}

void runVM(VM* vm) {

}