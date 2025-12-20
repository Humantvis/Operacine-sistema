#include "commandInterpreter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void interpretCommand(Kernel* kernel, const char* command) {
    if (command == NULL || strlen(command) == 0) {
        return;
    }

    if (strncmp(command, "run ", 4) == 0) {
        int program = (int)strtol(command + 4, NULL, 10);
        submitJob(kernel, program);
        return;
    }

    if (strcmp(command, "step") == 0) {
        kernel->rm->mode = STEP;
        return;
    }

    if (strcmp(command, "regs") == 0) {
        kernel->rm->mode = REGISTER_VALUES;
        return;
    }

    if (strcmp(command, "vm_mem") == 0) {
        kernel->rm->mode = VM_MEM;
        return;
    }

    printf("Unknown command: %s\n", command);
}