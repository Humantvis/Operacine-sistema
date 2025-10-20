#pragma once
#include <stdint.h>
#define WORD_SIZE 4
#define PAGE_SIZE 16
#define USER_MEMORY_SIZE 256
#define SUPERVIZOR_MEMORY_SIZE 256
#define SHARED_MEMORY 1

typedef struct{
    uint8_t userMemory[USER_MEMORY_SIZE*WORD_SIZE*PAGE_SIZE];
    uint8_t supervizorMemory[SUPERVIZOR_MEMORY_SIZE*WORD_SIZE*PAGE_SIZE];
    uint8_t sharedMemory[SHARED_MEMORY*WORD_SIZE*PAGE_SIZE];
} RM_Memory;


int initialize_RM_memory (RM_Memory* memory);
