#include "jobGovernor.h"
#include <stdlib.h>
#include <stdio.h>

void initJobGovernor(Kernel* kernel) {
    /* Nothing to initialize yet */
}

void submitJob(Kernel* kernel, int programId) {
    loadProgram(kernel->rm, kernel->memory, programId);

    VM_CPU* vm_cpu = malloc(sizeof(VM_CPU));

    VM* vm = malloc(sizeof(VM));
    if (!vm) {
        printf("[JobGovernor] VM allocation failed\n");
        return;
    }

    initVM(kernel->rm, vm, vm_cpu, programId, T_USER, kernel);

    kernel->readyUser->items[kernel->readyUser->count++] = vm;
}

void runJobGovernor(Kernel* kernel) {
    if (kernel->readyUser->count == 0)
        return;

    VM* vm = kernel->readyUser->items[0];

    for (int i = 1; i < kernel->readyUser->count; i++) {
        kernel->readyUser->items[i - 1] = kernel->readyUser->items[i];
    }
    kernel->readyUser->count--;

    kernel->runningProcess = vm;

    runVM(kernel->rm, vm);

    kernel->runningProcess = NULL;
}

void destroyJobGovernor(Kernel* kernel) {
    /* Optional cleanup */
}
