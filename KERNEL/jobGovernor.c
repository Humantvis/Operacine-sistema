#include "jobGovernor.h"

void runJobGovernor (Kernel* kernel, VM* vm) {
    runVM(kernel->rm, kernel->readyUser->items[kernel->readyUser->count]);
}
void initJobGovernor(Kernel* kernel, VM* vm) {
    kernel->readyUser->items[kernel->readyUser->count] = malloc(sizeof(VM));
    initVM(kernel->rm, kernel->readyUser->items[kernel->readyUser->count], kernel->readyUser->items[kernel->readyUser->count]->cpu, 0, T_USER, kernel);
}
void destroyJobGovernor(Kernel* kernel, VM* vm) {
    //destroylogic
}
