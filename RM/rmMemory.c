#include "rmMemory.h"

int initialize_RM_memory (RM_Memory* memory, SupervisorMemory* supervisorMemory) {
    memory->supervisorMemory = supervisorMemory;

    return 0;
}

void free_RM_memory(RM_Memory* memory) {
    free_RM_supervisorMemory(memory->supervisorMemory);
}