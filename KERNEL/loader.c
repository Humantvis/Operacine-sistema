#include "loader.h"
#include <stdio.h>

void loadProgram(RM* rm, ExternalMemory* externalMemory, int program) {
    int vm = -1;
    for(int i = 0; i < MAX_PROCESESSES; i++) {
        if(rm->memory->supervisorMemory->VMs[i] == false) {
            rm->memory->supervisorMemory->VMs[i] = true;
            vm = i;
            break;
        }
    }
    
    if(vm == -1) {
        printf("No available VM slots!\n");
        return;
    }

    for(int i = 0; i < TOTAL_MEMORY; i++) {
        rm->memory->supervisorMemory->VMPageTable[vm][i] = externalMemory->memory + (program * TOTAL_MEMORY_SIZE) + (i * PAGE_WORDS * WORD_SIZE);
    }
}