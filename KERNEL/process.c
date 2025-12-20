#include "process.h"

void initProcess(Process* process, int id, int priority, Kernel* kernel, VM* vm) {
    process->id = id;
    process->priority = priority;
    process->state = READY;
    process->kernel = kernel;
    process->vm = vm;
}
void startProcess(Process* process, int priority, int id) {
    process->kernel->runningProcess = process;
    if (process->vm!= NULL && process->priority == T_USER) {
        submitJob(process->kernel, process->id);
    }
    if (process->priority == T_SYSTEM) {
        switch(process->id) {
            case Scheduler:
                scheduler(process->kernel);
                break;
            case ReadFromInterface: {
                char* cmd = NULL;
                readFromInterface(&cmd); 
                if (cmd) interpretCommand(process->kernel, cmd);
                break;
            }
            case JobToSwap:
                jobToSwap(process);
                break;
            case MainProc:
                mainProc(process->kernel);
                break;
            case JobGovernor:
                runJobGovernor(process->kernel);
                break;
            case Loader:
                loadProgram(process->kernel->rm, process->kernel->memory, process->id);
                break;
            case InterruptHandler:
                interruptHandler(process->kernel);
                break;
            default:
                scheduler(process->kernel);
                break;
        }

    }
}
void changeState(Process* process, int newState) {
    process->state = newState;
    if (newState == RUNNING) {
        process->kernel->runningProcess = process;
        process->currentList = NULL;

        if (process->priority == T_SYSTEM) {
            process->kernel->readySystem->count--;
        } else {
            process->kernel->readyUser->count--;
        }
    }
}