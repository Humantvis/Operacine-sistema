#pragma once

#include "vmCpu.h"

#include "vmMemory.h"

#include "../RM/rm.h"

#include<stdlib.h>
#include<stdbool.h>

#include <stdio.h>
#include "debug.h"
#include "../RM/channelDevice.h"

#define FLAG_SF 0b00000001  // Sign flag
#define FLAG_ZF 0b00000010  // Zero flag
#define FLAG_CF 0b00000100  // Carry flag

typedef struct VM_CPU VM_CPU;

typedef struct VM {
    RM* rm;
    int id;
    VM_CPU* cpu;
    VM_Memory* memory;
} VM;

void initVM(RM* rm, VM* vm, VM_CPU* cpu, VM_Memory* memory, int id);

void destroyVM(VM* vm);

void runVM(RM* rm, VM* vm);

void executeInstruction(VM* vm, uint8_t instruction, RM* rm);

bool allowedToRun(RM* rm, VM* vm);

enum opCodes{
    //0:
    WAIT,
    SIGNAL,
    JNCxy,
    JCxy,
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