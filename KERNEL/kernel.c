#include "kernel.h"
#include "interruptHandler.h"
#include "../VM/debug.h"

void initKernel(Kernel* kernel, RM* rm, ExternalMemory* memory) {
    kernel->rm = rm;
    kernel->memory = memory;

    kernel->runningProcess = NULL;

    kernel->readySystem = malloc(sizeof(ProcessList));
    initProcessList(kernel->readySystem);

    kernel->readyUser = malloc(sizeof(ProcessList));
    initProcessList(kernel->readyUser);

    kernel->systemRunning = true;

    kernel->debugMode = RUN_FULL;
}

// start stop
void scheduler(Kernel* kernel) {

    /* ----- 1) Run ONE system process (if any) ----- */
    Process* sys = popProcess(kernel->readySystem);
    if (sys) {
        kernel->runningProcess = sys;
        changeState(sys, RUNNING);

        /* Run system process "one step" */
        startProcess(kernel->readySystem, sys, sys->priority, sys->id);

        /* If system process didn't change its own state, yield it */
        if (sys->state == RUNNING) {
            changeState(sys, READY);
        }

        kernel->runningProcess = NULL;

        /* Put it back only if it is READY */
        if (sys->state == READY) {
            insertProcess(kernel->readySystem, sys);
        }
        /* If it set itself BLOCKED/TERMINATED, we don't reinsert. */
    }

    /* ----- 2) Run ONE user process (if any) ----- */
    Process* p = popProcess(kernel->readyUser);
    if (!p) return;

    kernel->runningProcess = p;
    changeState(p, RUNNING);

    /* Debug policy (simple): if not RUN_FULL, show menu before running */
    if (kernel->debugMode != RUN_FULL) {
        debug(kernel, p->vm, kernel->rm->channelDevice);
    }

    /* Context switch */
    mountVM(kernel->rm, p->vm->id);

    runVM(kernel->rm, p->vm);

    unmountVM(kernel->rm);

    if (kernel->rm->cpu->si == SI_GET_DATA) {
        changeState(p, BLOCKED);
        kernel->runningProcess = NULL;
        return;
    }

    /* Handle halt */
    if (p->vm->halted) {
        finishProcess(kernel, p);
        kernel->runningProcess = NULL;
        return;
    }

    /* STEP: run only one instruction then give back console */
    if (kernel->debugMode == STEP) {
        changeState(p, READY);
        insertProcess(kernel->readyUser, p);
        kernel->runningProcess = NULL;
        return;
    }

    /* RUN_FULL: quantum */
    p->timeLeft--;
    if (p->timeLeft <= 0) {
        p->timeLeft = USER_QUANTUM;
        preemptProcess(kernel, p);
        kernel->runningProcess = NULL;
        return;
    }

    /* If still has time left, keep it runnable (round-robin) */
    changeState(p, READY);
    insertProcess(kernel->readyUser, p);
    kernel->runningProcess = NULL;
}