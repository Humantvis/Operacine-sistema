#include "mainProc.h"

void mainProc(Kernel* kernel) {
    initJobGovernor(kernel);
    runJobGovernor(kernel);
    destroyJobGovernor(kernel);

}