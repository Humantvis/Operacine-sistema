#include "externalMemory.h"

int initializeExternalmemory (ExternalMemory* memory) {
    memory->memory = calloc(EXTERNAL_MEMORY_SIZE * PAGE_SIZE, sizeof(uint8_t));
    if (!memory->memory) {
        perror("memory allocation failed");
        return 1;
    }
    return 0;
}

void freeExternalmemory(ExternalMemory* memory) {
    free(memory->memory);
}