#pragma once

#include "../defines.h"

typedef struct ExternalMemory {
    uint8_t* memory;
} ExternalMemory;


int initializeExternalmemory (ExternalMemory* memory);

void freeExternalmemory(ExternalMemory* memory);