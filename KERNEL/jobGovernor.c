#include "jobGovernor.h"
#include <stdlib.h>
#include <stdio.h>

void initJobGovernor(Kernel* kernel) {
    /* Nothing to initialize yet */
}

void submitJob(Kernel* kernel, int programId) {
    /* 1. Allocate VM CPU */
    VM_CPU* vm_cpu = malloc(sizeof(VM_CPU));
    if (!vm_cpu) {
        printf("[JobGovernor] VM_CPU allocation failed\n");
        return;
    }

    /* 2. Allocate VM */
    VM* vm = malloc(sizeof(VM));
    if (!vm) {
        printf("[JobGovernor] VM allocation failed\n");
        free(vm_cpu);
        return;
    }

    initVM(kernel->rm, vm, vm_cpu, programId);

    addNewVM(kernel->rm, vm->id);

    /* 3. Allocate Process */
    Process* p = malloc(sizeof(Process));
    if (!p) {
        printf("[JobGovernor] Process allocation failed\n");
        free(vm);
        free(vm_cpu);
        return;
    }

    /* 4. Initialize Process */
    initProcess(p, programId, T_USER, kernel, vm);

    /* 5. Insert into readyUser queue */
    insertProcess(kernel->readyUser, p);

    /* 6. Load program into VM memory */
    loadProgram(kernel->rm, kernel->memory, programId);
}

void destroyJobGovernor(Kernel* kernel) {
    /* Optional cleanup */
}
