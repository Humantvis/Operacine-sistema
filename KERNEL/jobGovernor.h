#pragma once
#include "kernel.h"

void initJobGovernor(Kernel* kernel);
void submitJob(Kernel* kernel, int programId);
void runJobGovernor(Kernel* kernel);
void destroyJobGovernor(Kernel* kernel);