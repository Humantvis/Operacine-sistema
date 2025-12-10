#pragma once

#include "supervisorMemory.h"
#include "../defines.h"

typedef struct RM_Memory {
    uint8_t userMemory[USER_MEMORY_SIZE];
    SupervisorMemory* supervisorMemory;
    uint8_t sharedMemory[SHARED_MEMORY_SIZE];
} RM_Memory;


int initialize_RM_memory(RM_Memory* memory, SupervisorMemory* supervisorMemory);

void free_RM_memory(RM_Memory* memory);