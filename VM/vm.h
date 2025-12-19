#pragma once

#include "vmCpu.h"

#include "../RM/rm.h"
#include "../KERNEL/kernel.h"

#include "debug.h"
#include "../RM/channelDevice.h"

#include "../defines.h"

typedef struct Kernel Kernel;
typedef struct ProcessList ProcessList;
typedef struct RM RM;
typedef struct VM_CPU VM_CPU;
typedef struct VM_Memory VM_Memory;
typedef struct VM VM;
typedef struct Channel_device Channel_device;

typedef struct VM {
    RM* rm;
    VM_CPU* cpu;

    int id;
    int priority;
    int state;
    ProcessList* currentList;
    Kernel* kernel;
} VM;

void initVM(RM* rm, VM* vm, VM_CPU* cpu, int id, int priority, Kernel* kernel);

void destroyVM(VM* vm);

void runVM(RM* rm, VM* vm);

void executeInstruction(VM* vm, uint8_t instruction, RM* rm, int execute);

int allowedToRun(RM* rm, VM* vm);