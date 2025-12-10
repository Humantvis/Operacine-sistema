#include "vmCpu.h"

void initVMCPU(VM_CPU* cpu, RM* rm) {
    for (int i = 0; i < REGISTERS; i++) {
        cpu->r[i] = &(rm->cpu->r[i]);
    }

    cpu->ic = &(rm->cpu->ic);
    cpu->offset = &(rm->cpu->offset);
    cpu->fr = &(rm->cpu->fr);
}

void setRegister(VM* vm, int reg, uint8_t value) {
    vm->rm->cpu->r[reg] = value;
}

uint8_t getRegister(VM* vm, int reg) {
    return vm->rm->cpu->r[reg];
}

void setIc(VM* vm, uint16_t ic) {
    vm->rm->cpu->ic = ic;
}

uint16_t getIc(VM* vm) {
    return vm->rm->cpu->ic;
}

void setOffset(VM* vm, int offset) {
    vm->rm->cpu->offset = offset;
}

int getOffset(VM* vm) {
    return vm->rm->cpu->offset;
}

void setFr(VM* vm, uint8_t fr) {
    vm->rm->cpu->fr = fr;
}

uint8_t getFr(VM* vm) {
    return vm->rm->cpu->fr;
}