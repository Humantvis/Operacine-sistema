#include "vm.h"

uint8_t readOpCode(VM* vm, RM* rm);

uint8_t readRegister(VM* vm, RM* rm);

uint8_t* readMemoryAddress(VM* vm, RM* rm);

void jump(VM* vm, RM* rm, uint8_t jumpLocation);

void noJump(VM* vm, RM* rm);

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
    int first = 1;
    debug(rm, vm, rm->channelDevice);
    while (allowedToRun(rm, vm)==1) {
        int action = rm->cpu->buffer[0];

        if (action == 0) {

            uint8_t instruction = readOpCode(vm, rm);

            if(instruction == HALT) {
            outputchannel(rm->channelDevice, "HALT");
                break;
            } 
            else {
                executeInstruction(vm, instruction, rm, 1);
            }
        } else {
            if(first == 1) {
                first = 0;
            } else {
                debug(rm, vm, rm ->channelDevice);
            }
            uint8_t instruction = readOpCode(vm, rm);
    
            if(instruction == HALT) {
                outputchannel(rm->channelDevice, "HALT");
                break;
            } 
            else {
                executeInstruction(vm, instruction, rm, 1);
            }

            //next command
            uint8_t* tempIc = getIc(vm);
            uint8_t tempOffset = getOffset(vm);
            instruction = readOpCode(vm, rm);
            if(instruction == HALT) {
                outputchannel(rm->channelDevice, "Next Instruction: HALT");
            } 
            else {
                executeInstruction(vm, instruction, rm, 0);
            }
            setIc(vm, tempIc);
            setOffset(vm, tempOffset);
        }
    }
}

void executeInstruction(VM* vm, uint8_t instruction, RM* rm, int execute) {
    char message[256]; // reusable buffer for all messages

    switch (instruction) {
        case ADDxyz: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);

            if (execute == 1)
            {
                setRegister(vm, z, getRegister(vm, x) + getRegister(vm, y));
    
                snprintf(message, sizeof(message),
                    "ADD: R%d = R%d + R%d -> R%d = %d + %d\n",
                    z, x, y, z, getRegister(vm, x), getRegister(vm, y));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instrution: ADD R%d R%d R%d \n",
                    x, y, z);
                outputchannel(rm->channelDevice, message);
            }

            break;
        }
        case SUBxyz: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);

            setRegister(vm, z, getRegister(vm, x) - getRegister(vm, y));

            if (execute == 1){
                snprintf(message, sizeof(message),
                    "SUB: R%d = R%d - R%d -> R%d = %d - %d\n",
                    z, x, y, z, getRegister(vm, x), getRegister(vm, y));

                outputchannel(rm->channelDevice, message);
            }
            else {
                    snprintf(message, sizeof(message),
                    "Next instruction: SUB R%d R%d R%d\n",
                    x, y, z);

                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case MULxyzw: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            uint8_t w = readRegister(vm, rm);

            uint16_t a = getRegister(vm, x);
            uint16_t b = getRegister(vm, y);
            uint16_t result = a * b;

            if (execute == 1) {
                setRegister(vm, z, result & 0xFF);
                setRegister(vm, w, (result >> 8) & 0xFF);

                snprintf(message, sizeof(message),
                "MUL: R%d = R%d * R%d -> R%d = %d * %d\n", z, x, y, z, getRegister(vm, x), getRegister(vm, y));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                "Next intruction: MUL R%d R%d R%d R%d\n", x, y, z, w);
                outputchannel(rm->channelDevice, message);
            }

            break;
        }
        case DIVxyzw: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);
            uint8_t z = readRegister(vm, rm);
            uint8_t w = readRegister(vm, rm);

            int dividend = getRegister(vm, x);
            int divisor = getRegister(vm, y);

            if (execute == 1) {
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
            }
            else {
                    snprintf(message, sizeof(message),
                    "Next instruction: DIV R%d R%d R%d R%d\n", x, y, z, w);
                    outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case CMPxy: {
            uint8_t x = readRegister(vm, rm);
            uint8_t y = readRegister(vm, rm);

            if (execute == 1) {
                if(getRegister(vm, x) == getRegister(vm, y)) {
                    setFr(vm, getFr(vm) | FLAG_ZF);
                } else if(getRegister(vm, x) < getRegister(vm, y)) {
                    setFr(vm, getFr(vm) | FLAG_SF);
                } else {
                    setFr(vm, getFr(vm) & ~FLAG_SF);
                    setFr(vm, getFr(vm) & ~FLAG_ZF);
                }
    
                snprintf(message, sizeof(message),
                    "CMP: R%d cmp R%d -> FR = %d\n", x, y, getFr(vm));
    
                outputchannel(rm->channelDevice, message);
            }
        else {
                snprintf(message, sizeof(message),
                    "Next instruction: CMP R%d R%d \n", x, y);
                outputchannel(rm->channelDevice, message);
        }
            break;
        }
        case MRxyz: {
            uint8_t* xy = readMemoryAddress(vm, rm);
            uint8_t z = readRegister(vm, rm);

            if (execute == 1) {
                setRegister(vm, z, *xy);
    
                snprintf(message, sizeof(message),
                    "MR: R%d = M[%ld] -> R%d = %d\n",
                    z, xy - vm->memory->dataMemory, z, *xy);
    
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: MR %1d R%d \n",
                    xy - vm->memory->dataMemory, z);
    
                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case MWxyz: {
            uint8_t* xy = readMemoryAddress(vm, rm);
            uint8_t z = readRegister(vm, rm);
            
            if (execute == 1) {
                *xy = getRegister(vm, z);
                snprintf(message, sizeof(message),
                    "MW: M[%ld] = R%d -> M[%ld] = %d\n",
                    xy - vm->memory->dataMemory, z,
                    xy - vm->memory->dataMemory, getRegister(vm, z));
    
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: MW %ld R%d\n",
                    xy - vm->memory->dataMemory, z);
    
                outputchannel(rm->channelDevice, message);
            }
            break;
        }

        case SMRxyz: {
            if (execute == 1)
            {
                snprintf(message, sizeof(message), "SMR: not implemented yet\n");
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message), "Next instruction: SMR \n");
                outputchannel(rm->channelDevice, message); 
            }
            break;

        }

        case SMWxyz: {
            if (execute == 1)
            {
                snprintf(message, sizeof(message), "SMW: not implemented yet\n");
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message), "Next instruction: SMW \n");
                outputchannel(rm->channelDevice, message); 
            }
            break;
        }

        case WAIT: {
            if (execute == 1)
            {
                snprintf(message, sizeof(message), "WAIT: not implemented yet\n");
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message), "Next instruction: WAIT \n");
                outputchannel(rm->channelDevice, message); 
            }
            break;
        }

        case SIGNAL: {
            if (execute == 1)
            {
                snprintf(message, sizeof(message), "SIGNAL: not implemented yet\n");
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message), "Next instruction: SIGNAL \n");
                outputchannel(rm->channelDevice, message); 
            }
            break;
        }

        case JMPxy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }

            if (execute == 1)
            {
                jump(vm, rm, jumpLocation);
                
                snprintf(message, sizeof(message),
                    "JMP: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                
                snprintf(message, sizeof(message),
                    "Next instruction: JMP %x \n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            
            break;
        }
        case JExy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }

            if (execute == 1) {
                if ((getFr(vm) & FLAG_ZF) == FLAG_ZF) {
                    jump(vm, rm, jumpLocation);
                } else{
                    noJump(vm, rm);
                }
                snprintf(message, sizeof(message),
                    "JE: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: JE %x\n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case JGxy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }
            
            if (execute == 1) {
                if ((getFr(vm) & FLAG_SF) == 0) {
                    jump(vm, rm, jumpLocation);
                } else{
                    noJump(vm, rm);
                }
                snprintf(message, sizeof(message),
                    "JG: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: JG %x \n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case JLxy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }

            if (execute == 1) {
                if (getFr(vm) & FLAG_SF) {
                    jump(vm, rm, jumpLocation);
                } else{
                    noJump(vm, rm);
                }
                snprintf(message, sizeof(message),
                    "JL: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "JL %x\n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case JLExy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }

            if (execute == 1) {
                if ((getFr(vm) & FLAG_SF) || (getFr(vm) & FLAG_ZF)) {
                    jump(vm, rm, jumpLocation);
                } else{
                    noJump(vm, rm);
                }
                snprintf(message, sizeof(message),
                    "JLE: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: JLE %x\n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case JCxy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }
            if (execute == 1) {
                if (getFr(vm) & FLAG_CF) {
                    jump(vm, rm, jumpLocation);
                } else{
                    noJump(vm, rm);
                }
                snprintf(message, sizeof(message),
                    "JC: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: JC %x \n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }
        case JNCxy: {
            uint8_t jumpLocation;

            if (getOffset(vm) <= 3) {
                jumpLocation = (*getIc(vm) >> (4 - getOffset(vm))) & 0b00001111;
            } else {
                jumpLocation = ((*getIc(vm) << (getOffset(vm) - 4)) | ((*(getIc(vm) + 1)) >> (12 - getOffset(vm)))) & 0b00001111;
            }

            if (execute == 1) {
                if (!(getFr(vm) & FLAG_CF)) {
                    jump(vm, rm, jumpLocation);
                } else{
                    noJump(vm, rm);
                }
                snprintf(message, sizeof(message),
                    "JNC: IC = %llx\n", getIc(vm));
                outputchannel(rm->channelDevice, message);
            }
            else {
                snprintf(message, sizeof(message),
                    "Next instruction: JNC %x\n", jumpLocation);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }

        case DMARx: {
            uint8_t* x = readMemoryAddress(vm, rm);  // pointer to memory
            if (execute == 1) {
                inputchannel(rm->channelDevice);  // fills rm->cpu->buffer[64]

                for (int i = 0; i < 64; i++) {
                    x[i] = rm->cpu->buffer[i];
                }

                snprintf(message, sizeof(message),
                        "DMAR: M[%p] = 64 bytes (16 words) from user input\n",
                        (void*)x);
                outputchannel(rm->channelDevice, message);
            } else {
                snprintf(message, sizeof(message),
                        "Next instruction: DMAR %p\n",
                        (void*)x);
                outputchannel(rm->channelDevice, message);
            }
            break;
        }

        case DMAWx: {
            uint8_t* x = readMemoryAddress(vm, rm);  // pointer to memory
            if (execute == 1) {
                char buf[128];
                for (int i = 0; i < 64; i++) {
                    // Print each byte in hex into buffer
                    snprintf(buf, sizeof(buf), "%02X ", x[i]);
                    outputchannel(rm->channelDevice, buf);
                }

                snprintf(message, sizeof(message),
                        "DMAW: M[%p] = 64 bytes (16 words) printed\n",
                        (void*)x);
                outputchannel(rm->channelDevice, message);
            } else {
                snprintf(message, sizeof(message),
                        "Next instruction: DMAW %p\n",
                        (void*)x);
                outputchannel(rm->channelDevice, message);
            }
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

uint8_t* readMemoryAddress(VM* vm, RM* rm) {
    uint8_t address = (((*getIc(vm) << getOffset(vm)) | ((*(getIc(vm) + 1)) >> (8 - getOffset(vm)))) & 0b11111111);

    setIc(vm, getIc(vm) + 1);

    return vm->memory->dataMemory + address;
}

void jump(VM* vm, RM* rm, uint8_t jumpLocation) {

    size_t base_index = CODE_MEMORY * PAGE_SIZE - jumpLocation * 9;

    uint64_t addr_val = 0;
    for (int i = 0; i < 8; i++) {
        addr_val |= ((uint64_t)*(vm->memory->codeMemory + base_index + i)) << (i * 8);
    }

    uint8_t* jumpAddress = (uint8_t*)(uintptr_t)addr_val;

    uint8_t storedOffset = *(vm->memory->codeMemory + base_index + 8);
    setOffset(vm, storedOffset);

    setIc(vm, jumpAddress);
}

void noJump(VM* vm, RM* rm) {
    setOffset(vm, getOffset(vm) + 4);
    if(getOffset(vm) >= 8) {
        setOffset(vm, getOffset(vm) % 8);
        setIc(vm, getIc(vm) + 1);
    }
}

int allowedToRun(RM* rm, VM* vm) {
    if  (rm->cpu->mountedVMID == vm->id)
    {
        return 1;
    }
    else return 0;
}