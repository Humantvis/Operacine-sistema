#include "kernel.h"

void initKernel(Kernel* kernel, RM* rm)
{
    kernel -> rm = rm;
    kernel->readySystem = malloc(sizeof(ProcessList));
    initList(kernel->readySystem, T_SYSTEM, kernel);
    kernel->readyUser = malloc(sizeof(ProcessList));
    initList(kernel->readyUser, T_USER, kernel);

}
void scheduler (Kernel* kernel)
{

}