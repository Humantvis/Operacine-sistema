#pragma once

#include "../defines.h"

typedef struct SupervisorMemory {
    bool VMs[MAX_PROCESESSES];
    int mountedVMID;
    uint8_t buffer[BUFFER_SIZE];

    uint8_t* VMPageTable[MAX_PROCESESSES][CODE_MEMORY + DATA_MEMORY + FREE_MEMORY];

    uint8_t stateR[MAX_PROCESESSES][REGISTERS];

    uintptr_t stateCi[MAX_PROCESESSES];

    uint8_t stateOffset[MAX_PROCESESSES];

    uint8_t stateFr[MAX_PROCESESSES];
} SupervisorMemory;

int initialize_RM_supervisorMemory (SupervisorMemory* memory);

void free_RM_supervisorMemory(SupervisorMemory* memory);