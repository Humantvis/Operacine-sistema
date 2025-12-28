#pragma once
#include "../defines.h"
#include "../RM/rm.h"
#include "../VM/vm.h"
#include "loader.h"
#include "jobGovernor.h"
#include "processList.h"
#include "process.h"
#include "startStop.h"
#include "readFromInterface.h"
#include "commandInterpreter.h"

typedef struct VM VM;
typedef struct RM RM;
typedef struct Process Process;

typedef struct Kernel {
    Process* runningProcess;

    ProcessList* readySystem;
    ProcessList* readyUser;

    RM* rm;
    ExternalMemory* memory;

    bool systemRunning;

    int debugMode;
} Kernel;

void initKernel(Kernel* kernel, RM* rm, ExternalMemory* memory);
void scheduler(Kernel* kernel);