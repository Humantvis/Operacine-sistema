#include "interruptHandler.h"
#include "process.h"
#include <stdio.h>

void handleInterrupts(Kernel* kernel) {
    Process* p = kernel->runningProcess;
    if (!p) return;

    switch (kernel->rm->cpu->pi) {
        case PI_NONE:
            break;

        case PI_INVALID_OPCODE:
        case PI_INVALID_ADDRESS:
        case PI_OVERFLOW:
        case PI_ILLEGAL_ASSIGNMENT:
            printf("Program Interrupt: terminating process %d\n", p->id);

            kernel->rm->cpu->pi = PI_NONE;
            kernel->rm->cpu->si = SI_NONE;
            finishProcess(kernel, p);
            return;
    }

    switch (kernel->rm->cpu->si) {
        case SI_NONE:
            break;

        case SI_GET_DATA:
            kernel->rm->cpu->pi = PI_NONE;
            kernel->rm->cpu->si = SI_NONE;

            changeState(p, BLOCKED_STOP);
            return;

        case SI_PUT_DATA:
            kernel->rm->cpu->pi = PI_NONE;
            kernel->rm->cpu->si = SI_NONE;
            changeState(p, BLOCKED_STOP);
            return;

        case SI_HALT:
            kernel->rm->cpu->pi = PI_NONE;
            kernel->rm->cpu->si = SI_NONE;
            finishProcess(kernel, p);
            return;
    }
}