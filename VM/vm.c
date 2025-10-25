#include "vm.h"

uint8_t readOpCode(VM* vm, RM* rm);

uint8_t readRegister(VM* vm, RM* rm);

uint8_t* readCodeAddress(VM* vm, RM* rm);

uint8_t* readMemoryAddress(VM* vm, RM* rm);

void initVM(RM* rm, VM* vm, VM_CPU* cpu, VM_Memory* memory, int id) {
    vm->rm = rm;
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
        uint8_t instruction = readOpCode(vm, rm);
        if(instruction == HALT) {
            printf("HALT\n");
            break;
        } else {
            executeInstruction(vm, instruction, rm);
        }
    }
}

void executeInstruction(VM* vm, uint8_t instruction, RM* rm) {
    switch(instruction) {
        case ADDxyz: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            setRegister(vm, z, getRegister(vm, x) + getRegister(vm, y));
            printf("ADD: R%d = R%d + R%d -> R%d = %d + %d\n", z, x, y, z, getRegister(vm, x), getRegister(vm, y));
            break;
        }
        case SUBxyz: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            setRegister(vm, z, getRegister(vm, x) - getRegister(vm, y));
            printf("SUB: R%d = R%d - R%d -> R%d = %d - %d\n", z, x, y, z, getRegister(vm, x), getRegister(vm, y));
            break;
        }
        case MULxyzw: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            uint8_t w = readRegister(vm, rm);
            setRegister(vm, z, getRegister(vm, x) * getRegister(vm, y));
            setRegister(vm, w, getRegister(vm, x) * getRegister(vm, y)); // cia kaz kas ne taip
            printf("MUL: R%d = R%d * R%d -> R%d = %d * %d\n", z, x, y, z, getRegister(vm, x), getRegister(vm, y));
            break;
        }
        case DIVxyzw: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            uint8_t w = readRegister(vm, rm);
            setRegister(vm, z, getRegister(vm, x) / getRegister(vm, y));
            setRegister(vm, w, getRegister(vm, x) % getRegister(vm, y));
            printf("DIV: R%d = R%d / R%d -> R%d = %d / %d,   R%d = R%d %% R%d -> R%d = %d %% %d\n", z, x, y, z, getRegister(vm, z), getRegister(vm, x), w, x, y, getRegister(vm, w), getRegister(vm, x), getRegister(vm, y));
            break;
        }
        case CMPxy: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            if(getRegister(vm, x) == getRegister(vm, y)) {
                setFr(vm, getFr(vm) | FLAG_ZF);
            } else if(getRegister(vm, x) < getRegister(vm, y)) {
                setFr(vm, getFr(vm) | FLAG_SF);
            } else {
                setFr(vm, getFr(vm) & ~FLAG_SF);
                setFr(vm, getFr(vm) & ~FLAG_ZF);
            }
            printf("CMP: R%d cmp R%d -> FR = %d\n", x, y, getFr(vm));
            break;
        }
        case MRxyz: {
            uint8_t* xy = readMemoryAddress(vm, rm);
            uint8_t z = readRegister(vm, rm);
            setRegister(vm, z, *xy);
            printf("MR: R%d = M[%d] -> R%d = %d\n", z, xy - vm->memory->dataMemory, z, *xy);
            break;
        }
        case MWxyz: {
            uint8_t* xy = readMemoryAddress(vm, rm);
            uint8_t z = readRegister(vm, rm);
            *xy = getRegister(vm, z);
            printf("MW: M[%d] = R%d -> M[%d] = %d\n", xy - vm->memory->dataMemory, z, xy - vm->memory->dataMemory, getRegister(vm, z));
            break;
        }
        case SMRxyz: {
            //not needed yet
            printf("SMR: not implemented yet\n");
            break;
        }
        case SMWxyz: {
            //not needed yet
            printf("SMW: not implemented yet\n");
            break;
        }
        case WAIT: {
            //semaphores are not needed yet
            printf("WAIT: not implemented yet\n");
            break;
        }
        case SIGNAL: {
            //semaphores are not needed yet
            printf("SIGNAL: not implemented yet\n");
            break;
        }
        case JMPxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            setIc(vm, xy);
            printf("JMP: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case JExy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if ((getFr(vm) & FLAG_ZF) == FLAG_ZF) {
                setIc(vm, xy);
            }
            printf("JE: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case JGxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if ((getFr(vm) & FLAG_SF) == 0) {
                setIc(vm, xy);
            }
            printf("JG: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case JLxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if (getFr(vm) & FLAG_SF) {
                setIc(vm, xy);
            }
            printf("JL: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case JLExy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if ((getFr(vm) & FLAG_SF) || (getFr(vm) & FLAG_ZF)) {
                setIc(vm, xy);
            }
            printf("JLE: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case JCxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if (getFr(vm) & FLAG_CF) {
                setIc(vm, xy);
            }
            printf("JC: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case JNCxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if (!(getFr(vm) & FLAG_CF)) {
                setIc(vm, xy);
            }
            printf("JNC: IC = %d\n", xy - vm->memory->codeMemory);
            break;
        }
        case DMARx: {
            //not needed yet
            printf("DMAR: not implemented yet\n");
            break;
        }
        case DMAWx: {
            //not needed yet
            printf("DMAW: not implemented yet\n");
            break;
        }
    }
}

uint8_t readOpCode(VM* vm, RM* rm) {
    uint8_t instruction;
    if(getOffset(vm) <= 3) {
        instruction = (*getIc(vm) >> (3 - getOffset(vm))) & 0b00011111;
    } else {
        instruction = ((*getIc(vm) << (getOffset(vm) - 3)) | (*(getIc(vm) + 1) >> (11 - getOffset(vm)))) & 0b00011111;
    }

    setOffset(vm, getOffset(vm) + 5);
    if(getOffset(vm) >= 8) {
        setOffset(vm, getOffset(vm) % 8);
        setIc(vm, getIc(vm) + 1);
    }
    
    return instruction;
}

uint8_t readRegister(VM* vm, RM* rm) {
    uint8_t reg;
    if(getOffset(vm) <= 3) {
        reg = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
    } else {
        reg = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
    }

    setOffset(vm, getOffset(vm) + 4);
    if(getOffset(vm) >= 8) {
        setOffset(vm, getOffset(vm) % 8);
        setIc(vm, getIc(vm) + 1);
    }

    return reg;
}

uint8_t* readCodeAddress(VM* vm, RM* rm) {
    uint8_t address = (((*getIc(vm) << getOffset(vm)) | ((*(getIc(vm) + 1)) >> (8 - getOffset(vm)))) & 0b11111111);

    setIc(vm, getIc(vm) + 1);

    return vm->memory->codeMemory + address;
}

uint8_t* readMemoryAddress(VM* vm, RM* rm) {
    uint8_t address = (((*getIc(vm) << getOffset(vm)) | ((*(getIc(vm) + 1)) >> (8 - getOffset(vm)))) & 0b11111111);

    setIc(vm, getIc(vm) + 1);

    return vm->memory->dataMemory + address;
}

bool allowedToRun(RM* rm, VM* vm) {
    return rm->cpu->mountedVMID == vm->id;
}