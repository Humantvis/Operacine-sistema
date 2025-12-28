#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define WORD_SIZE 4
#define PAGE_WORDS 16

#define DATA_MEMORY 4 
#define CODE_MEMORY 8
#define FREE_MEMORY 3
#define SHARED_MEMORY 1

#define PAGE_COUNT 16
#define PAGE_SIZE (PAGE_WORDS * WORD_SIZE)

#define TOTAL_MEMORY (DATA_MEMORY + CODE_MEMORY + FREE_MEMORY)
#define TOTAL_MEMORY_SIZE (TOTAL_MEMORY * PAGE_SIZE)

#define REGISTERS 16

#define FLAG_SF 0b00000001  // Sign flag
#define FLAG_ZF 0b00000010  // Zero flag
#define FLAG_CF 0b00000100  // Carry flag

#define MAX_PROCESESSES 16

#define SHARED_MEMORY_SIZE (SHARED_MEMORY * PAGE_SIZE)
#define USER_MEMORY_SIZE (MAX_PROCESESSES * TOTAL_MEMORY_SIZE + SHARED_MEMORY_SIZE)

#define BUFFER_SIZE 64

#define EXTERNAL_MEMORY_SIZE (64 * PAGE_SIZE)

#define USER_QUANTUM 3

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

enum Memory_Segments {
    DATA,
    CODE
};

enum RM_Modes {
    SUPER,
    USER
};

enum Actions {
    RUN_FULL,
    STEP,
    REGISTER_VALUES,
    VM_MEM
};

enum ProcessState {
    RUNNING,
    READY,
    BLOCKED,
    READY_STOP,
    BLOCKED_STOP,
    TERIMINATED
};

enum ProcessType {
    T_SYSTEM,
    T_USER
};

enum ProgramInterrupt {
    PI_NONE,
    PI_INVALID_OPCODE,
    PI_INVALID_ADDRESS,
    PI_OVERFLOW,
    PI_ILLEGAL_ASSIGNMENT
};

enum SupervisorInterrupt{
    SI_NONE,
    SI_GET_DATA,
    SI_PUT_DATA,
    SI_HALT
};

typedef enum ResourceType {
    RES_CPU,
    RES_CHANNEL,
    RES_PAGING,
    RES_EXTERNAL_MEMORY,
    RES_INPUT,
    RES_COUNT
} ResourceType;

enum SystemProcesses{
    Scheduler,
    ReadFromInterface,
    JobToSwap,
    Loader,
    InterruptHandler
};