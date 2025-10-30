#include "rmMemory.h"
int initialize_RM_memory (RM_Memory* memory) {
    memory->userMemory = calloc(USER_MEMORY_SIZE * WORD_SIZE * PAGE_SIZE, sizeof(uint8_t));
    if (!memory->userMemory) {
        perror("userMemory allocation failed");
        return 1;
    }

    memory->supervizorMemory = calloc(SUPERVIZOR_MEMORY_SIZE * WORD_SIZE * PAGE_SIZE, sizeof(uint8_t));
    if (!memory->supervizorMemory) {
        perror("supervizorMemory allocation failed");
        free(memory->userMemory);
        return 1;
    }

    memory->sharedMemory = calloc(SHARED_MEMORY * WORD_SIZE * PAGE_SIZE, sizeof(uint8_t));
    if (!memory->sharedMemory) {
        perror("sharedMemory allocation failed");
        free(memory->userMemory);
        free(memory->supervizorMemory);
        return 1;
    }

    return 0;
}

void free_RM_memory(RM_Memory* memory) {
    free(memory->userMemory);
    free(memory->supervizorMemory);
    free(memory->sharedMemory);
}