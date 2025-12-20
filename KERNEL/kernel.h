#pragma once
#include "../defines.h"
#include "../RM/rm.h"
#include "../VM/vm.h"
#include "loader.h"
#include "jobGovernor.h"
#include "mainProc.h"
#include "processList.h"

typedef struct VM VM;
typedef struct RM RM;

typedef struct Kernel {
    VM* runningProcess;

    ProcessList* readySystem;
    ProcessList* readyUser;

    //resursu manageris

    //ProgramInterrupt pi;
    //SupervisorInterrupt si;

    RM* rm;
    ExternalMemory* memory;

} Kernel;

void initKernel(Kernel* kernel, RM* rm, ExternalMemory* memory);
void scheduler(Kernel* kernel);