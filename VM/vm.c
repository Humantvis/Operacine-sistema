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

        if (rm->cpu->buffer != 0)
        {
            debug(rm, vm, rm ->channelDevice);
            uint8_t action = rm->cpu->buffer;
            if(action ==1)
            {
                uint8_t instruction = readOpCode(vm, rm);
    
                if(instruction == HALT) {
                    outputchannel(rm->channelDevice, "HALT");
                    break;
                } 
                else {
                    executeInstruction(vm, instruction, rm);
                }
            }
            
        }
        else if (rm->cpu->buffer == 0) {

                uint8_t instruction = readOpCode(vm, rm);

                if(instruction == HALT) {
                outputchannel(rm->channelDevice, "HALT");
                    break;
                } 
                else {
                    executeInstruction(vm, instruction, rm);
                }
        }
    }
}

void executeInstruction(VM* vm, uint8_t instruction, RM* rm) {
    char message[256]; // reusable buffer for all messages

    switch (instruction) {
        case ADDxyz: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);

            setRegister(vm, z, getRegister(vm, x) + getRegister(vm, y));

            snprintf(message, sizeof(message),
                "ADD: R%d = R%d + R%d -> R%d = %d + %d\n",
                z, x, y, z, getRegister(vm, x), getRegister(vm, y));

            outputchannel(rm->channelDevice, message);
            break;
        }

        case SUBxyz: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);

            setRegister(vm, z, getRegister(vm, x) - getRegister(vm, y));

            snprintf(message, sizeof(message),
                "SUB: R%d = R%d - R%d -> R%d = %d - %d\n",
                z, x, y, z, getRegister(vm, x), getRegister(vm, y));

            outputchannel(rm->channelDevice, message);
            break;
        }

        case MULxyzw: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            uint8_t w = readRegister(vm, rm);

            int result = getRegister(vm, x) * getRegister(vm, y);
            setRegister(vm, z, result);
            setRegister(vm, w, result); // TODO: check if this logic is correct

            snprintf(message, sizeof(message),
                "MUL: R%d = R%d * R%d -> R%d = %d * %d\n",
                z, x, y, z, getRegister(vm, x), getRegister(vm, y));

            outputchannel(rm->channelDevice, message);
            break;
        }

        case DIVxyzw: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            uint8_t w = readRegister(vm, rm);

            int dividend = getRegister(vm, x);
            int divisor  = getRegister(vm, y);

            if (divisor == 0) {
                snprintf(message, sizeof(message), "DIV ERROR: Division by zero!\n");
            } else {
                setRegister(vm, z, dividend / divisor);
                setRegister(vm, w, dividend % divisor);
                snprintf(message, sizeof(message),
                    "DIV: R%d = R%d / R%d -> %d / %d,  R%d = R%d %% R%d -> %d %% %d\n",
                    z, x, y, dividend, divisor,
                    w, x, y, dividend, divisor);
            }

            outputchannel(rm->channelDevice, message);
            break;
        }

        case CMPxy: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            int fr = getFr(vm);

            if (getRegister(vm, x) == getRegister(vm, y)) {
                setFr(vm, fr | FLAG_ZF);
            } else if (getRegister(vm, x) < getRegister(vm, y)) {
                setFr(vm, fr | FLAG_SF);
            } else {
                setFr(vm, fr & ~FLAG_SF);
                setFr(vm, fr & ~FLAG_ZF);
            }

            snprintf(message, sizeof(message),
                "CMP: R%d cmp R%d -> FR = %d\n", x, y, getFr(vm));

            outputchannel(rm->channelDevice, message);
            break;
        }

        case MRxyz: {
            uint8_t* xy = readMemoryAddress(vm, rm);
            uint8_t z = readRegister(vm, rm);
            setRegister(vm, z, *xy);

            snprintf(message, sizeof(message),
                "MR: R%d = M[%ld] -> R%d = %d\n",
                z, xy - vm->memory->dataMemory, z, *xy);

            outputchannel(rm->channelDevice, message);
            break;
        }

        case MWxyz: {
            uint8_t* xy = readMemoryAddress(vm, rm);
            uint8_t z = readRegister(vm, rm);
            *xy = getRegister(vm, z);

            snprintf(message, sizeof(message),
                "MW: M[%ld] = R%d -> M[%ld] = %d\n",
                xy - vm->memory->dataMemory, z,
                xy - vm->memory->dataMemory, getRegister(vm, z));

            outputchannel(rm->channelDevice, message);
            break;
        }

        case SMRxyz:
            snprintf(message, sizeof(message), "SMR: not implemented yet\n");
            outputchannel(rm->channelDevice, message);
            break;

        case SMWxyz:
            snprintf(message, sizeof(message), "SMW: not implemented yet\n");
            outputchannel(rm->channelDevice, message);
            break;

        case WAIT:
            snprintf(message, sizeof(message), "WAIT: not implemented yet\n");
            outputchannel(rm->channelDevice, message);
            break;

        case SIGNAL:
            snprintf(message, sizeof(message), "SIGNAL: not implemented yet\n");
            outputchannel(rm->channelDevice, message);
            break;

        case JMPxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            setIc(vm, xy);
            snprintf(message, sizeof(message),
                "JMP: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case JExy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if ((getFr(vm) & FLAG_ZF) == FLAG_ZF) {
                setIc(vm, xy);
            }
            snprintf(message, sizeof(message),
                "JE: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case JGxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if ((getFr(vm) & FLAG_SF) == 0) {
                setIc(vm, xy);
            }
            snprintf(message, sizeof(message),
                "JG: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case JLxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if (getFr(vm) & FLAG_SF) {
                setIc(vm, xy);
            }
            snprintf(message, sizeof(message),
                "JL: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case JLExy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if ((getFr(vm) & FLAG_SF) || (getFr(vm) & FLAG_ZF)) {
                setIc(vm, xy);
            }
            snprintf(message, sizeof(message),
                "JLE: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case JCxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if (getFr(vm) & FLAG_CF) {
                setIc(vm, xy);
            }
            snprintf(message, sizeof(message),
                "JC: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case JNCxy: {
            uint8_t* xy = readCodeAddress(vm, rm);
            if (!(getFr(vm) & FLAG_CF)) {
                setIc(vm, xy);
            }
            snprintf(message, sizeof(message),
                "JNC: IC = %ld\n", xy - vm->memory->codeMemory);
            outputchannel(rm->channelDevice, message);
            break;
        }

        case DMARx:
            snprintf(message, sizeof(message), "DMAR: not implemented yet\n");
            outputchannel(rm->channelDevice, message);
            break;

        case DMAWx:
            snprintf(message, sizeof(message), "DMAW: not implemented yet\n");
            outputchannel(rm->channelDevice, message);
            break;
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