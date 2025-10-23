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
        uint8_t* instr_ptr = vm->memory->codeMemory + (*(vm->cpu->ic));
        uint8_t instruction = (*instr_ptr >> 3) & 0b00011111;

        executeInstruction(vm, instruction);

        (*(vm->cpu->ic))++;
    }
}

bool allowedToRun(RM* rm, VM* vm) {
    return rm->cpu->mountedVMID == vm->id;
}
void executeInstruction(VM* vm, uint8_t instruction) {

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
            uint8_t w = mem[++(*ic)];
            vm->cpu->r[z] = vm->cpu->r[x] * vm->cpu->r[y];
            break;
        }
        case DIVxyzw: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            uint8_t w = mem[++(*ic)];
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
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->cpu->r[z] = vm->memory->dataMemory[x * 16 + y];
            break;
        }
        case MWxyz: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->memory->dataMemory[x * 16 + y] = vm->cpu->r[z];
            break;
        }
        case SMRxyz: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->cpu->r[z] = vm->memory->sharedMemory[x * 16 + y];
            break;
        }
        case SMWxyz: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            uint8_t z = mem[++(*ic)];
            vm->memory->sharedMemory[x * 16 + y] = vm->cpu->r[z];
            break;
        }
        case WAIT: {
            //semaphores are not needed yet
            break;
        }
        case SIGNAL: {
            //semaphores are not needed yet
            break;
        }
        case JMPxy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            *(vm->cpu->ic) = x * 16 + y;
            break;
        }
        case JExy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            if (vm->cpu->fr == 1) {
                *(vm->cpu->ic) = x * 16 + y;
            }
            break;
        }
        case JGxy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            if ((vm->cpu->fr & FLAG_SF) == 0) 
                *ic = x * 16 + y;
            break;
        }
        case JLxy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];

        if (vm->cpu->fr & FLAG_SF) 
        *(vm->cpu->ic) = x * 16 + y;
            break;
        }
        case JLExy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];

        if ((vm->cpu->fr & FLAG_SF) || (vm->cpu->fr & FLAG_ZF))
                *(vm->cpu->ic) = x * 16 + y;
            break;
        }
        case JCxy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];
            if (vm->cpu->fr & FLAG_CF) 
                *(vm->cpu->ic) = x * 16 + y;
            break;
        }
        case JNCxy: {
            uint8_t x = mem[++(*ic)];
            uint8_t y = mem[++(*ic)];

            if (!(vm->cpu->fr & FLAG_CF)) 
                *(vm->cpu->ic) = x * 16 + y;
            break;
        }
        case DMARx: {
            uint8_t x = mem[++(*ic)];
            //need to implement instruction reading first
            break;
        }
        case DMAWx: {
            uint8_t x = mem[++(*ic)];
            //need to implement instruction writing first
            break;
        }
        case HALT: {
            destroyVM(vm);
            break;
        }
    }
}
