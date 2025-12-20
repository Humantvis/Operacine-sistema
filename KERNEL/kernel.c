#include "kernel.h"

void initKernel(Kernel* kernel, RM* rm, ExternalMemory* memory) {
    kernel->rm = rm;
    kernel->memory = memory;

    kernel->runningProcess = NULL;

    kernel->readySystem = malloc(sizeof(ProcessList));
    initList(kernel->readySystem, T_SYSTEM, kernel);

    kernel->readyUser = malloc(sizeof(ProcessList));
    initList(kernel->readyUser, T_USER, kernel);
}

//iskviestas start stop
void scheduler(Kernel* kernel) {
    while(true) {
        changeState(kernel->readySystem->items[InterruptHandler], READY);
        startProcess(kernel->runningProcess, T_SYSTEM, InterruptHandler);
        char *cmd = NULL;
        startProcess(kernel->runningProcess, T_SYSTEM, ReadFromInterface);
        if (cmd != NULL) {
            interpretCommand(kernel, cmd);
        }
        changeState(kernel->runningProcess, BLOCKED);

        if (kernel->readySystem->count > 0) {
            mainProc(kernel);
            continue;
        }

        if (kernel->readyUser->count > 0) {
            runJobGovernor(kernel);
            continue;
        }
    }
}