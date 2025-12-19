#include "supervisorMemory.h"

int initialize_RM_supervisorMemory (SupervisorMemory* memory) {
    memory->mountedVMID = -1;
    for (int i = 0; i < MAX_PROCESESSES; i++) {
        memory->VMs[i] = false;
        for (int j = 0; j < CODE_MEMORY + DATA_MEMORY + FREE_MEMORY; j++) {
            memory->VMPageTable[i][j] = (uint8_t*)NULL;
        }
    }
    for (int i = 0; i < BUFFER_SIZE; i++) {
        memory->buffer[i] = 0;
    }
    return 0;
}

void free_RM_supervisorMemory(SupervisorMemory* memory) {
    free(memory);
}