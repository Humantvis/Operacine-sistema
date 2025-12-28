#pragma once

#include "../defines.h"
#include "kernel.h"
#include "processList.h"

typedef struct Resource {
    const char* name;
    bool busy;
    Process* owner;
    ProcessList* waitQ;
} Resource;

typedef struct ResourceManager {
    Kernel* kernel;
    Resource resources[RES_COUNT];
} ResourceManager;

bool requestResource(ResourceManager* rm, ResourceType type, Process* p);
void releaseResource(ResourceManager* rm, ResourceType type, Process* owner);
