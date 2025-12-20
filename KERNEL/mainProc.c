#include "mainProc.h"

void mainProc(Kernel* kernel) {
    //jcl tikrinimas
    if (kernel->rm->cpu->pi)
    initJobGovernor(kernel, kernel->readyUser->items[0]);
    runJobGovernor(kernel, kernel->readyUser->items[0]);
    destroyJobGovernor(kernel, kernel->readyUser->items[0]);

}