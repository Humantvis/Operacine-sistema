#include "jobGovernor.h"
#include <stdlib.h>
#include <stdio.h>

void initJobGovernor(Kernel* kernel) {
    /* Nothing to initialize yet */
}

void submitJob(Kernel* kernel, int programId) {
    VM_CPU* vm_cpu = malloc(sizeof(VM_CPU));
    
    VM* vm = malloc(sizeof(VM));
    if (!vm) {
        printf("[JobGovernor] VM allocation failed\n");
        return;
    }
    
    initVM(kernel->rm, vm, vm_cpu, programId);
    
    kernel->readyUser->items[kernel->readyUser->count++]->vm = vm;
    initProcess(kernel->readyUser->items[kernel->readyUser->count], programId, T_USER, kernel, kernel->readyUser->items[kernel->readyUser->count]->vm);
    loadProgram(kernel->rm, kernel->memory, programId);
}

void runJobGovernor(Kernel* kernel) {
    if (kernel->readyUser->count == 0)
        return;

    VM* vm = kernel->readyUser->items[0]->vm;

    for (int i = 1; i < kernel->readyUser->count; i++) {
        kernel->readyUser->items[i - 1] = kernel->readyUser->items[i];
    }
    kernel->readyUser->count--;
    kernel->runningProcess->vm = vm;

    runVM(kernel->rm, vm);
}

void destroyJobGovernor(Kernel* kernel) {
    /* Optional cleanup */
}
