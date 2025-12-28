#pragma once

#include "../defines.h"
#include "process.h"

typedef struct Process Process;

typedef struct ProcessList {
    Process* items[MAX_PROCESESSES];
    int count;
} ProcessList;

void initProcessList(ProcessList* list);

void insertProcess(ProcessList* list, Process* p);

Process* popProcess(ProcessList* list);

bool removeProcess(ProcessList* list, Process* p);
bool containsProcess(ProcessList* list, Process* p);