#include "process.h"
#include "interruptHandler.h"

void initProcess(Process* process, int id, int priority, Kernel* kernel, VM* vm) {
    process->id = id;
    process->priority = priority;
    process->state = READY;
    process->timeLeft = 0;
    process->currentList = NULL;
    process->kernel = kernel;
    process->vm = vm;

    process->debugInitialized = false;
    process->lastDebugMode = RUN_FULL;
}

void startProcess(ProcessList* processList, Process* process, int priority, int id) {
    (void)priority;
    (void)id;

    if (process->vm != NULL && process->priority == T_USER) {
        submitJob(process->kernel, process->id);
        return;
    }

    if (process->priority == T_SYSTEM) {
        switch (process->id) {
            case ReadFromInterface: {
                char* cmd = NULL;
                readFromInterface(&cmd);
                if (cmd) {
                    interpretCommand(process->kernel, cmd);
                }
                break;
            }

            case JobToSwap:
                insertProcess(processList, process);
                break;

            case Loader:
                loadProgram(process->kernel->rm, process->kernel->memory, process->id);
                break;

            case InterruptHandler:
                handleInterrupts(process->kernel);
                break;

            default:
                break;
        }
    }
}

void changeState(Process* process, int newState) {
    process->state = newState;

    if (newState == RUNNING) {
        process->kernel->runningProcess = process;
        process->currentList = NULL;
    }
}

void preemptProcess(Kernel* kernel, Process* p) {
    changeState(p, READY);
    kernel->runningProcess = NULL;

    insertProcess(kernel->readyUser, p);
}

void finishProcess(Kernel* kernel, Process* p) {
    changeState(p, TERIMINATED);
    kernel->runningProcess = NULL;
}