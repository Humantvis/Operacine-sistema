#include "startStop.h"
void start(Kernel* kernel) {
    Process* p;

    p = malloc(sizeof(Process));
    initProcess(p, ReadFromInterface, T_SYSTEM, kernel, NULL);
    kernel->readySystem->items[kernel->readySystem->count++] = p;
    changeState(p, BLOCKED);

    p = malloc(sizeof(Process));
    initProcess(p, JobToSwap, T_SYSTEM, kernel, NULL);
    kernel->readySystem->items[kernel->readySystem->count++] = p;
    changeState(p, BLOCKED);

    p = malloc(sizeof(Process));
    initProcess(p, MainProc, T_SYSTEM, kernel, NULL);
    kernel->readySystem->items[kernel->readySystem->count++] = p;
    changeState(p, BLOCKED);

    p = malloc(sizeof(Process));
    initProcess(p, Loader, T_SYSTEM, kernel, NULL);
    kernel->readySystem->items[kernel->readySystem->count++] = p;
    changeState(p, BLOCKED);

    p = malloc(sizeof(Process));
    initProcess(p, InterruptHandler, T_SYSTEM, kernel, NULL);
    kernel->readySystem->items[kernel->readySystem->count++] = p;
    changeState(p, BLOCKED);

    kernel->runningProcess = malloc(sizeof(Process));
    initProcess(kernel->runningProcess, Scheduler, T_SYSTEM, kernel, NULL);
    startProcess(kernel->runningProcess, T_SYSTEM, Scheduler);
    changeState(p, RUNNING);
}
