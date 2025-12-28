#include "vm.h"

uint8_t readOpCode(VM* vm, RM* rm);

uint8_t readRegister(VM* vm, RM* rm);

uint8_t* readMemoryAddress(VM* vm, RM* rm);

void jump(VM* vm, RM* rm, uint8_t jumpLocation);

void noJump(VM* vm, RM* rm);

// Read an 8-bit immediate from the code stream (without translating it to VM memory).
static uint8_t readImm8(VM* vm, RM* rm) {
    uint16_t ic = getIc(vm);
    int codePage = ic / PAGE_SIZE;
    int codeOff  = ic % PAGE_SIZE;

    uint8_t* codeBase = rm->pagingDevice->getAddressFromPage(
                            rm->pagingDevice, vm->id, codePage);

    uint8_t byte0 = codeBase[codeOff];
    uint8_t byte1 = codeBase[(codeOff + 1) % PAGE_SIZE];

    uint8_t first  = (uint8_t)(byte0 << getOffset(vm));
    uint8_t second = (uint8_t)(byte1 >> (8 - getOffset(vm)));

    uint8_t value = (uint8_t)((first | second) & 0xFF);

    // consume 8 bits (byte) from the instruction stream
    setIc(vm, (uint16_t)(getIc(vm) + 1));

    return value;
}

void initVM(RM* rm, VM* vm, VM_CPU* cpu, int id) {
    vm->rm = rm;
    vm->cpu = cpu;
    vm->id = id;
    vm->halted = false;
}

void destroyVM(VM* vm) {
    if (vm == NULL) {
        return;
    }

    free(vm->cpu);
    free(vm);
}

void runVM(RM* rm, VM* vm) {

    rm->cpu->pi = PI_NONE;
    rm->cpu->si = SI_NONE;


    Kernel* kernel = rm->kernel;

    /* Show debug menu ONLY if:
       - first time running this VM
       - OR previous mode was not RUN_FULL */
    if (kernel->debugMode != RUN_FULL) {
        debug(kernel, vm, rm->channelDevice);
    }

    rm->cpu->pi = PI_NONE;
    rm->cpu->si = SI_NONE;

    if (allowedToRun(rm, vm) != 1) {
        return;
    }

    uint8_t instruction = readOpCode(vm, rm);

    if (instruction == HALT) {
        outputchannel(rm->channelDevice, "HALT\n");
        rm->cpu->si = SI_HALT;
        rm->cpu->pi = PI_NONE;
        vm->halted = true;
        return;
    }

    executeInstruction(vm, instruction, rm, 1);

    /* Stop immediately on interrupt */
    if (rm->cpu->pi != PI_NONE || rm->cpu->si != SI_NONE) {
        return;
    }

    /* STEP mode: return control to console */
    if (kernel->debugMode == STEP) {
        kernel->debugMode = STEP;   // keep mode
        return;
    }

    /* RUN_FULL continues normally */
}

void executeInstruction(VM* vm, uint8_t instruction, RM* rm, int execute) {
    char message[256];

    switch (instruction) {

    /* ==================== ARITHMETIC ==================== */

    case ADDxyz: {
        uint8_t x = readRegister(vm, rm);
        uint8_t y = readRegister(vm, rm);
        uint8_t z = readRegister(vm, rm);

        if (execute) {
            int sum = getRegister(vm, x) + getRegister(vm, y);
            if (sum > 255) rm->cpu->pi = PI_OVERFLOW;
            setRegister(vm, z, (uint8_t)sum);

            snprintf(message, sizeof(message),
                "ADD: R%d = R%d + R%d -> %d\n",
                z, x, y, sum);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: ADD R%d R%d R%d\n", x, y, z);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    case SUBxyz: {
        uint8_t x = readRegister(vm, rm);
        uint8_t y = readRegister(vm, rm);
        uint8_t z = readRegister(vm, rm);

        if (execute) {
            int res = getRegister(vm, x) - getRegister(vm, y);
            if (res < 0) rm->cpu->pi = PI_OVERFLOW;
            setRegister(vm, z, (uint8_t)res);

            snprintf(message, sizeof(message),
                "SUB: R%d = R%d - R%d -> %d\n",
                z, x, y, res);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: SUB R%d R%d R%d\n", x, y, z);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    case MULxyzw: {
        uint8_t x = readRegister(vm, rm);
        uint8_t y = readRegister(vm, rm);
        uint8_t z = readRegister(vm, rm);
        uint8_t w = readRegister(vm, rm);

        if (execute) {
            uint16_t res = getRegister(vm, x) * getRegister(vm, y);
            setRegister(vm, z, res & 0xFF);
            setRegister(vm, w, (res >> 8) & 0xFF);

            snprintf(message, sizeof(message),
                "MUL: R%d:R%d = R%d * R%d -> %u\n",
                w, z, x, y, res);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: MUL R%d R%d R%d R%d\n", x, y, z, w);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    case DIVxyzw: {
        uint8_t x = readRegister(vm, rm);
        uint8_t y = readRegister(vm, rm);
        uint8_t z = readRegister(vm, rm);
        uint8_t w = readRegister(vm, rm);

        if (execute) {
            int d = getRegister(vm, y);
            if (d == 0) {
                rm->cpu->pi = PI_ILLEGAL_ASSIGNMENT;
                snprintf(message, sizeof(message), "DIV ERROR: divide by zero\n");
            } else {
                int q = getRegister(vm, x) / d;
                int r = getRegister(vm, x) % d;
                setRegister(vm, z, q);
                setRegister(vm, w, r);

                snprintf(message, sizeof(message),
                    "DIV: R%d=%d R%d=%d\n", z, q, w, r);
            }
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: DIV R%d R%d R%d R%d\n", x, y, z, w);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    /* ==================== LOGIC ==================== */

    case CMPxy: {
        uint8_t x = readRegister(vm, rm);
        uint8_t y = readRegister(vm, rm);

        if (execute) {
            uint8_t fr = 0;
            if (getRegister(vm, x) == getRegister(vm, y)) fr |= FLAG_ZF;
            if (getRegister(vm, x) < getRegister(vm, y)) fr |= FLAG_SF;
            setFr(vm, fr);

            snprintf(message, sizeof(message),
                "CMP: R%d ? R%d -> FR=%02X\n", x, y, fr);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: CMP R%d R%d\n", x, y);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    /* ==================== MEMORY ==================== */

    case MRxyz: {
        uint8_t* addr = readMemoryAddress(vm, rm);
        uint8_t z = readRegister(vm, rm);

        if (execute) {
            setRegister(vm, z, *addr);
            snprintf(message, sizeof(message),
                "MR: R%d = M[%p]\n", z, (void*)addr);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: MR %p R%d\n", (void*)addr, z);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    case MWxyz: {
        uint8_t* addr = readMemoryAddress(vm, rm);
        uint8_t z = readRegister(vm, rm);

        if (execute) {
            *addr = getRegister(vm, z);
            snprintf(message, sizeof(message),
                "MW: M[%p] = R%d\n", (void*)addr, z);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: MW %p R%d\n", (void*)addr, z);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    case SMRxyz: {
        uint8_t a = readImm8(vm, rm);
        uint8_t z = readRegister(vm, rm);

        if (execute) {
            setRegister(vm, z, rm->memory->sharedMemory[a]);
            snprintf(message, sizeof(message),
                "SMR: R%d = SHARED[%u]\n", z, a);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: SMR %u R%d\n", a, z);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    case SMWxyz: {
        uint8_t a = readImm8(vm, rm);
        uint8_t z = readRegister(vm, rm);

        if (execute) {
            rm->memory->sharedMemory[a] = getRegister(vm, z);
            snprintf(message, sizeof(message),
                "SMW: SHARED[%u] = R%d\n", a, z);
        } else {
            snprintf(message, sizeof(message),
                "Next instruction: SMW %u R%d\n", a, z);
        }
        outputchannel(rm->channelDevice, message);
        break;
    }

    /* ==================== SYNCHRONIZATION ==================== */

    case WAIT: {
        if (execute) {
            if (rm->sharedSemaphore == 1) {
                rm->sharedSemaphore = 0;
                outputchannel(rm->channelDevice, "WAIT: acquired\n");
            } else {
                outputchannel(rm->channelDevice, "WAIT: blocked\n");

                rm->cpu->si = SI_GET_DATA;   // or a dedicated SI_WAIT
            }
        }
        break;
    }

    case SIGNAL:
        if (execute) {
            rm->sharedSemaphore = 1;
            snprintf(message, sizeof(message), "SIGNAL: released\n");
        } else {
            snprintf(message, sizeof(message), "Next instruction: SIGNAL\n");
        }
        outputchannel(rm->channelDevice, message);
        break;

    /* ==================== INVALID ==================== */

    default:
        rm->cpu->pi = PI_INVALID_OPCODE;
        snprintf(message, sizeof(message),
            "INVALID OPCODE: %u\n", instruction);
        outputchannel(rm->channelDevice, message);
        break;
    }
}


uint8_t readOpCode(VM* vm, RM* rm) {
    uint16_t ic = getIc(vm);
    int page = ic / PAGE_SIZE;

    uint8_t* base0 = rm->pagingDevice->getAddressFromPage(rm->pagingDevice, vm->id, page);
    if (!base0) {
        rm->cpu->pi = PI_INVALID_ADDRESS;
        return HALT; // or 0; but HALT is convenient to stop
    }

    // If instruction crosses page boundary, next byte might be on next page
    int page1 = (ic + 1) / PAGE_SIZE;
    uint8_t* base1 = rm->pagingDevice->getAddressFromPage(rm->pagingDevice, vm->id, page1);
    if (!base1) {
        rm->cpu->pi = PI_INVALID_ADDRESS;
        return HALT;
    }

    uint8_t* phys0 = base0 + (ic % PAGE_SIZE);
    uint8_t* phys1 = base1 + ((ic + 1) % PAGE_SIZE);

    uint8_t off = (uint8_t)getOffset(vm);

    // sanity: offset must be 0..7
    if (off > 7) {
        rm->cpu->pi = PI_INVALID_ADDRESS;
        off %= 8;
        setOffset(vm, off);
    }

    uint8_t first = 0, second = 0;

    if (off <= 3) {
        first = (*phys0 >> (3 - off));
    } else {
        first = (*phys0 << (off - 3));
        second = (*phys1 >> (11 - off));
    }

    uint8_t instruction = (first | second) & 0b00011111;

    setOffset(vm, off + 5);
    if (getOffset(vm) >= 8) {
        setOffset(vm, getOffset(vm) % 8);
        setIc(vm, getIc(vm) + 1);
    }

    return instruction;
}

uint8_t readRegister(VM* vm, RM* rm) {
    uint16_t ic = getIc(vm);

    int page0 = ic / PAGE_SIZE;
    int page1 = (ic + 1) / PAGE_SIZE;

    uint8_t* base0 = rm->pagingDevice->getAddressFromPage(rm->pagingDevice, vm->id, page0);
    uint8_t* base1 = rm->pagingDevice->getAddressFromPage(rm->pagingDevice, vm->id, page1);

    if (!base0 || !base1) {
        rm->cpu->pi = PI_INVALID_ADDRESS;
        return 0;
    }

    uint8_t* phys0 = base0 + (ic % PAGE_SIZE);
    uint8_t* phys1 = base1 + ((ic + 1) % PAGE_SIZE);

    uint8_t off = (uint8_t)getOffset(vm);
    if (off > 7) off %= 8;

    uint8_t first = 0, second = 0;

    if (off <= 3) {
        first = (*phys0 >> (4 - off));
    } else {
        first = (*phys0 << (off - 4));
        second = (*phys1 >> (12 - off));
    }

    uint8_t reg = (first | second) & 0b00001111;

    setOffset(vm, off + 4);
    if (getOffset(vm) >= 8) {
        setOffset(vm, getOffset(vm) % 8);
        setIc(vm, getIc(vm) + 1);
    }

    return reg;
}

void jump(VM* vm, RM* rm, uint8_t jumpLocation) {
    size_t vmBase = vm->id * TOTAL_MEMORY_SIZE;

    size_t jumpTableBase =
        vmBase + (DATA_MEMORY + CODE_MEMORY) * PAGE_WORDS * WORD_SIZE;

    size_t entryIndex = jumpTableBase - (jumpLocation * 9);

    uint8_t* entryPtr = rm->memory->userMemory + entryIndex;

    uint64_t icVal = 0;

    for (int i = 0; i < 8; i++) {
        icVal |= ((uint64_t)entryPtr[i]) << (i * 8);
    }

    uint8_t storedOffset = entryPtr[8];

    setIc(vm, (uint16_t)icVal);
    setOffset(vm, storedOffset);
}

uint8_t* readMemoryAddress(VM* vm, RM* rm) {
    uint16_t ic = getIc(vm);
    int codePage = ic / PAGE_SIZE;
    int codeOff  = ic % PAGE_SIZE;

    uint8_t* codeBase = rm->pagingDevice->getAddressFromPage(
                            rm->pagingDevice, vm->id, codePage);

    uint8_t byte0 = codeBase[codeOff];
    uint8_t byte1 = codeBase[(codeOff + 1) % PAGE_SIZE];

    uint8_t first  = byte0 << getOffset(vm);
    uint8_t second = byte1 >> (8 - getOffset(vm));

    uint8_t address = (first | second) & 0xFF;

    setIc(vm, getIc(vm) + 1);
    
    uint16_t vaddr = address;

    int dataPage = vaddr / PAGE_SIZE;
    int dataOff  = vaddr % PAGE_SIZE;

    uint8_t* dataBase = rm->pagingDevice->getAddressFromPage(
                            rm->pagingDevice, vm->id, dataPage);

    return dataBase + dataOff;
}


void noJump(VM* vm, RM* rm) {
    setOffset(vm, getOffset(vm) + 4);
    if(getOffset(vm) >= 8) {
        setOffset(vm, getOffset(vm) % 8);
        setIc(vm, getIc(vm) + 1);
    }
}

int allowedToRun(RM* rm, VM* vm) {
    if  (rm->memory->supervisorMemory->mountedVMID == vm->id) {
        return 1;
    }
    else return 0;
}