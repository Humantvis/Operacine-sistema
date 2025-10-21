#include "vm.h"


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

void runVM(RM* rm, VM* vm) {
    while (allowedToRun(rm, vm)) {
        uint8_t* instr_ptr = vm->memory->codeMemory + (*(vm->cpu->ic)) * WORD_SIZE;
        uint32_t instruction = *(uint32_t*)instr_ptr;

        executeInstruction(vm, instruction);

        (*(vm->cpu->ic))++;
    }
}

bool allowedToRun(RM* rm, VM* vm) {
    return rm->cpu->mountedVMID == vm->id;
}
void executeInstruction(VM* vm, uint32_t instruction) {

    uint8_t* mem = vm->memory->codeMemory;
    uint8_t* ic = vm->cpu->ic;

    switch(instruction) {
        case ADDxyz: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->cpu->r[x] = vm->cpu->r[y] + vm->cpu->r[z];
            break;
        }
        case SUBxyz: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->cpu->r[z] = vm->cpu->r[x] - vm->cpu->r[y];
            break;
        }
        case MULxyzw: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->cpu->r[z] = vm->cpu->r[x] * vm->cpu->r[y];
            break;
        }
        case DIVxyzw: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->cpu->r[z] = vm->cpu->r[x] / vm->cpu->r[y];
            break;
        }
        case CMPxy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            vm->cpu->fr = (vm->cpu->r[x] == vm->cpu->r[y]) ? 1 : 0;
            break;
        }
        case MRxyz: {
            break;
        }
        case MWxyz: {
            break;
        }
        case SMRxyz: {
            break;
        }
        case SMWxyz: {
            break;
        }
        case WAIT: {
            break;
        }
        case SIGNAL: {
            break;
        }
        case JMPxy: {
            break;
        }
        case JExy: {
            break;
        }
        case JGxy: {
            break;
        }
        case JLxy: {
            break;
        }
        case JLExy: {
            break;
        }
        case JC: {
            break;
        }
        case JNC: {
            break;
        }
        case DMARx: {
            break;
        }
        case DMAWx: {
            break;
        }
        case HALT: {
            break;
        }
    }
}
enum opCodes{
    //0:
    WAIT,
    SIGNAL,
    JNC,
    JC,
    HALT,
    //1:
    DMARx,
    DMAWx,
    //2:
    CMPxy,
    JMPxy,
    JExy,
    JNExy,
    JGxy,
    JGExy,
    JLxy,
    JLExy,
    //3:
    ADDxyz,
    SUBxyz,
    MRxyz,
    MWxyz,
    SMRxyz,
    SMWxyz,
    //4:
    MULxyzw,
    DIVxyzw
};