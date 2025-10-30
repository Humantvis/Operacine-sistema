#pragma once
#include <stdlib.h>
#include <stdint.h>
#define WORD_SIZE 4
#define PAGE_SIZE 16
#define USER_MEMORY_SIZE (256 * 2)
#define SUPERVIZOR_MEMORY_SIZE 256
#define SHARED_MEMORY 1

typedef struct RM_Memory {
    uint8_t* userMemory;
    uint8_t* supervizorMemory;
    uint8_t* sharedMemory;
} RM_Memory;


int initialize_RM_memory (RM_Memory* memory);

void free_RM_memory(RM_Memory* memory);