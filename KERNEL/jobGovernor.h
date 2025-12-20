#pragma once

#include "../defines.h"
#include "kernel.h"

void runJobGovernor (Kernel* kernel, VM* vm);
void initJobGovernor (Kernel* kernel, int job);
void destroyJobGovernor (Kernel* kernel, VM* vm);