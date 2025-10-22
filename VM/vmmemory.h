#pragma once
#include <stdint.h>
#include "../RM/rmMemory.h"
#define WORD_SIZE 4
#define DATA_MEMORY 4 
#define CODE_MEMORY 8
#define FREE_MEMORY 3
#define PAGE_TOTAL_WORDS 16
#define SHARED_MEMORY 1

#define TOTAL_MEMORY_SIZE (DATA_MEMORY + CODE_MEMORY + FREE_MEMORY) * PAGE_TOTAL_WORDS * WORD_SIZE

#define PAGE_COUNT 16
#define PAGE_SIZE 16

typedef struct {
    uint8_t *dataMemory;
    uint8_t *codeMemory;
    uint8_t *freeMemory;
    uint8_t *sharedMemory;
} VM_Memory;


int initializeVirtualMemory (VM_Memory* vmMemory, uint8_t* datapointer);