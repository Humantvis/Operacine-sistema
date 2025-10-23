#include "vm.h"
uint8_t readOpCode(VM* vm);

uint8_t readRegister(VM* vm);

uint8_t readAddress(VM* vm);

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
        uint8_t instruction = readOpCode(vm); 
        executeInstruction(vm, instruction);
    }
}

bool allowedToRun(RM* rm, VM* vm) {
    return rm->cpu->mountedVMID == vm->id;
}
void executeInstruction(VM* vm, uint8_t instruction) {

    switch(instruction) {
        case ADDxyz: {
            uint8_t x = readRegister(vm);
            uint8_t y = readRegister(vm);
            uint8_t z = readRegister(vm);
            vm->cpu->r[z] = vm->cpu->r[x] + vm->cpu->r[y];
            break;
        }
        case SUBxyz: {
            uint8_t x = readRegister(vm);
            uint8_t y = readRegister(vm);
            uint8_t z = readRegister(vm);
            vm->cpu->r[z] = vm->cpu->r[x] - vm->cpu->r[y];
            break;
        }
        case MULxyzw: {
            uint8_t x = readRegister(vm);
            uint8_t y = readRegister(vm);
            uint8_t z = readRegister(vm);
            uint8_t w = readRegister(vm);
            vm->cpu->r[z] = vm->cpu->r[x] * vm->cpu->r[y];
            vm->cpu->r[w] = vm->cpu->r[x] * vm->cpu->r[y];
            break;
        }
        case DIVxyzw: {
            uint8_t x = readRegister(vm);
            uint8_t y = readRegister(vm);
            uint8_t z = readRegister(vm);
            uint8_t w = readRegister(vm);
            vm->cpu->r[z] = vm->cpu->r[x] / vm->cpu->r[y];
            vm->cpu->r[w] = vm->cpu->r[x] % vm->cpu->r[y];
            break;
        }
        case CMPxy: {
            uint8_t x = readRegister(vm);
            uint8_t y = readRegister(vm);
            if(vm->cpu->r[x] == vm->cpu->r[y])
                vm->cpu->fr |= FLAG_ZF;
            else if(vm->cpu->r[x] < vm->cpu->r[y])
                vm->cpu->fr |= FLAG_SF;
            else {
                vm->cpu->fr &= ~FLAG_SF;
                vm->cpu->fr &= ~FLAG_ZF;
            }
            break;
        }
        case MRxyz: {
            uint8_t xy = readAddress(vm);
            uint8_t z = readRegister(vm);
            vm->cpu->r[z] = vm->memory->dataMemory[xy];
            break;
        }
        case MWxyz: {
            uint8_t xy = readAddress(vm);
            uint8_t z = readRegister(vm);
            vm->memory->dataMemory[xy] = vm->cpu->r[z];
            break;
        }
        case SMRxyz: {
            //not needed yet
            break;
        }
        case SMWxyz: {
            //not needed yet
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
            uint8_t xy = readAddress(vm);
            *(vm->cpu->ic) = xy;
            break;
        }
        case JExy: {
            uint8_t xy = readAddress(vm);
            if ((vm->cpu->fr & FLAG_ZF) == FLAG_ZF) {
                *(vm->cpu->ic) = xy;
            }
            break;
        }
        case JGxy: {
            uint8_t xy = readAddress(vm);
            if ((vm->cpu->fr & FLAG_SF) == 0) 
                *(vm->cpu->ic) = xy;
            break;
        }
        case JLxy: {
            uint8_t xy = readAddress(vm);
        if (vm->cpu->fr & FLAG_SF) 
        *(vm->cpu->ic) = xy;
            break;
        }
        case JLExy: {
            uint8_t xy = readAddress(vm);
        if ((vm->cpu->fr & FLAG_SF) || (vm->cpu->fr & FLAG_ZF))
                *(vm->cpu->ic) = xy;
            break;
        }
        case JCxy: {
            uint8_t xy = readAddress(vm);
            if (vm->cpu->fr & FLAG_CF) 
                *(vm->cpu->ic) = xy;
            break;
        }
        case JNCxy: {
            uint8_t xy = readAddress(vm);

            if (!(vm->cpu->fr & FLAG_CF)) 
                *(vm->cpu->ic) = xy;
            break;
        }
        case DMARx: {
            //not needed yet
            break;
        }
        case DMAWx: {
            //not needed yet
            break;
        }
        case HALT: {
            destroyVM(vm);
            break;
        }
    }
}
/*uint8_t readOpCode(VM* vm) {
    return (*(vm->memory->codeMemory + (*(vm->cpu->ic))) & 0b11111000) >> 3;
}

uint8_t readRegister1(VM* vm) {
    return ((*(vm->memory->codeMemory + (*(vm->cpu->ic))) & 0b00000111) << 1) | ((*(vm->memory->codeMemory + (*(vm->cpu->ic) + 1)) & 0b10000000) >> 7);
}

uint8_t readRegister2(VM* vm) {
        return(*(vm->memory->codeMemory + (*(vm->cpu->ic)+1)) & 0b01111000) >> 3;
}

uint8_t readRegister3(VM* vm) {
    return ((*(vm->memory->codeMemory + (*(vm->cpu->ic)+1)) & 0b00000111) << 1) | ((*(vm->memory->codeMemory + (*(vm->cpu->ic) + 2)) & 0b10000000) >> 7);
}

uint8_t readRegister4(VM* vm) {
    return(*(vm->memory->codeMemory + (*(vm->cpu->ic)+2)) & 0b01111000) >> 3;
}

uint8_t readAddress1(VM* vm) {
    return ((*(vm->memory->codeMemory + (*(vm->cpu->ic))) & 0b00011111)<< 3) | ((*(vm->memory->codeMemory + (*(vm->cpu->ic) + 1)) & 0b11100000) >> 5);
}*/
uint8_t readOpCode(VM* vm) {
    if(vm->cpu->offset <= 3) {
        return (vm->memory->codeMemory[*(vm->cpu->ic)] >> (3 - vm->cpu->offset)) & 0b00011111;
    } else {
        return ((vm->memory->codeMemory[*(vm->cpu->ic)] << (vm->cpu->offset - 3)) | (vm->memory->codeMemory[*(vm->cpu->ic) + 1] >> (11 - vm->cpu->offset))) & 0b00011111;
    }

    vm->cpu->offset += 5;
    if(vm->cpu->offset >= 8) {
        vm->cpu->offset = vm->cpu->offset % 8;
        (vm->cpu->ic)++;
    }
}

uint8_t readRegister(VM* vm) {
    if(vm->cpu->offset <= 3) {
        return (vm->memory->codeMemory[*(vm->cpu->ic)] >> (4 - vm->cpu->offset)) & 0b00001111;
    } else {
        return ((vm->memory->codeMemory[*(vm->cpu->ic)] << (vm->cpu->offset - 4)) | (vm->memory->codeMemory[*(vm->cpu->ic) + 1] >> (12 - vm->cpu->offset))) & 0b00001111;
    }

    vm->cpu->offset += 4;
    if(vm->cpu->offset >= 8) {
        vm->cpu->offset = vm->cpu->offset % 8;
        (vm->cpu->ic)++;
    }
}

uint8_t readAddress(VM* vm) {
    return ((vm->memory->codeMemory[*(vm->cpu->ic)] << vm->cpu->offset) | (vm->memory->codeMemory[*(vm->cpu->ic) + 1] >> (8 - vm->cpu->offset)));

    (vm->cpu->ic)++;
}