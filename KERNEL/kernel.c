#include "kernel.h"

void initKernel(Kernel* kernel, RM* rm, ExternalMemory* memory)
{
    kernel -> rm = rm;
    kernel->readySystem = malloc(sizeof(ProcessList));
    initList(kernel->readySystem, T_SYSTEM, kernel);
    kernel->readyUser = malloc(sizeof(ProcessList));
    initList(kernel->readyUser, T_USER, kernel);
    kernel->memory = memory;

}
void scheduler (Kernel* kernel)
{
    while (1)
    {
        //startstop
        //readfrominterface
        int choice = 0;
        loadProgram(kernel->rm, kernel->memory, choice);
        
        mainProc(kernel);



    }
}