#pragma once

#include "vmCpu.h"

#include "vmMemory.h"

#include "../RM/rm.h"

#include<stdlib.h>
#include<stdbool.h>

#define FLAG_SF 0x01  // Sign flag
#define FLAG_ZF 0x02  // Zero flag
#define FLAG_CF 0x04  // Carry flag


typedef struct VM {
    int id;
    VM_CPU* cpu;
    VM_Memory* memory;
} VM;

void initVM(VM* vm, VM_CPU* cpu, VM_Memory* memory, int id);

void destroyVM(VM* vm);

void runVM(RM* rm, VM* vm);

void executeInstruction(VM* vm, uint8_t instruction);

bool allowedToRun(RM* rm, VM* vm);

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