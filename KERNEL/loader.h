#pragma once

#include "../defines.h"
#include "../RM/externalMemory.h"
#include "../RM/rm.h"
typedef struct RM RM;

void loadProgram(RM* rm, ExternalMemory* externalMemory, int program);