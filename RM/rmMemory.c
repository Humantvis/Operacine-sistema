#include "rmMemory.h"
int initialize_RM_memory (RM_Memory* memory) {
    for (int i = 0; i < USER_MEMORY_SIZE * WORD_SIZE * PAGE_SIZE; i++) {
        memory->userMemory[i] = 0;
    }

    for (int i = 0; i < SUPERVIZOR_MEMORY_SIZE * WORD_SIZE * PAGE_SIZE; i++) {
        memory->supervizorMemory[i] = 0;
    }

    for (int i = 0; i < SHARED_MEMORY * WORD_SIZE * PAGE_SIZE; i++) {
        memory->sharedMemory[i] = 0;
    }

    return 0;
}
