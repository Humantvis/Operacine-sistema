#pragma once

#include "../defines.h"
#include "kernel.h"
#include "processList.h"

typedef struct Resource {
    const char* name;
    bool busy;
    VM* owner;
    ProcessList* waitQ;
} Resource;

typedef struct ResourceManager {
    Kernel* kernel;
    Resource resources[RES_COUNT];
} ResourceManager;

void initResourceManager(ResourceManager* rm);
void destroyResourceManager(ResourceManager* rm);

bool requestResource(ResourceManager* rm, ResourceType type, VM* vm);

void releaseResource(ResourceManager* rm, ResourceType type, VM* owner);
