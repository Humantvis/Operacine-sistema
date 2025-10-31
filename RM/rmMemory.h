#pragma once
#include <stdlib.h>
#include <stdint.h>
#define USER_MEMORY_SIZE 256
#define SUPERVIZOR_MEMORY_SIZE 256
#define SHARED_MEMORY 1

#define WORD_SIZE 4
#define DATA_MEMORY 4 
#define CODE_MEMORY 8
#define FREE_MEMORY 3
#define PAGE_WORDS 16
#define SHARED_MEMORY 1

#define TOTAL_MEMORY_SIZE ((DATA_MEMORY + CODE_MEMORY + FREE_MEMORY) * PAGE_WORDS * WORD_SIZE)

#define PAGE_COUNT 16
#define PAGE_SIZE (PAGE_WORDS * WORD_SIZE)

typedef struct RM_Memory {
    uint8_t* userMemory;
    uint8_t* supervizorMemory;
    uint8_t* sharedMemory;
} RM_Memory;


int initialize_RM_memory (RM_Memory* memory);

void free_RM_memory(RM_Memory* memory);