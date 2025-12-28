#pragma once
#include "kernel.h"

typedef struct Kernel Kernel;

void initJobGovernor(Kernel* kernel);
void submitJob(Kernel* kernel, int programId);
void destroyJobGovernor(Kernel* kernel);