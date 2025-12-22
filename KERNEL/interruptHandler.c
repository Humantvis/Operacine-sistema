#include "interruptHandler.h"

void handleInterrupts(Kernel* kernel) {
    switch(kernel->rm->cpu->pi) {
        case PI_NONE:
            break;
        case PI_INVALID_OPCODE:
            printf("Program Interrupt: Invalid Opcode\n");
            changeState(kernel->runningProcess, READY_STOP);
            break;
        case PI_INVALID_ADDRESS:
            printf("Program Interrupt: Invalid Address\n");
            changeState(kernel->runningProcess, READY_STOP);
            break; 
        case PI_OVERFLOW:
            printf("Program Interrupt: Overflow\n");
            changeState(kernel->runningProcess, READY_STOP);
            break;
        case PI_ILLEGAL_ASSIGNMENT:
            printf("Program Interrupt: Illegal Assignment\n");
            changeState(kernel->runningProcess, READY_STOP);
            break;
    }
    switch (kernel->rm->cpu->si) {
        case SI_NONE:
            break;
        case SI_GET_DATA:
            printf("Supervisor Interrupt: Get Data\n");
            changeState(kernel->runningProcess, SI_GET_DATA);
            break;
        case SI_PUT_DATA:
            printf("Supervisor Interrupt: Put Data\n");
            changeState(kernel->runningProcess, SI_PUT_DATA);
            break;
        case SI_HALT:
            printf("Supervisor Interrupt: Halt\n");
            changeState(kernel->runningProcess, READY_STOP);
            break;
    }
}
