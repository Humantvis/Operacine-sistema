#include "loader.h"
#include <stdio.h>
#include <string.h> //memcpy

void loadProgram(RM* rm, ExternalMemory* externalMemory, int program) {

    int vm = -1;
    for (int i = 0; i < MAX_PROCESESSES; i++) {
        if (rm->memory->supervisorMemory->VMs[i] == false) {
            rm->memory->supervisorMemory->VMs[i] = true;
            vm = i;
            break;
        }
    }

    if (vm == -1) {
        printf("[Loader] No available VM slots!\n");
        return;
    }

    for (int page = 0; page < TOTAL_MEMORY; page++) {

        uint8_t* chosen = NULL;

        for (int j = 0; j + PAGE_SIZE <= USER_MEMORY_SIZE; j += PAGE_SIZE) {

            uint8_t* candidate = &rm->memory->userMemory[j];
            bool used = false;

            for (int m = 0; m < MAX_PROCESESSES && !used; m++) {
                for (int n = 0; n < TOTAL_MEMORY; n++) {
                    if (rm->memory->supervisorMemory->VMPageTable[m][n] == candidate) {
                        used = true;
                        break;
                    }
                }
            }

            if (!used) {
                chosen = candidate;
                break;
            }
        }

        if (chosen == NULL) {
            printf("[Loader] ERROR: No free user pages in RM memory\n");
            return;
        }

        uint8_t* src = &externalMemory->memory[
            (program * TOTAL_MEMORY_SIZE) + (page * PAGE_SIZE)
        ];

        memcpy(chosen, src, PAGE_SIZE);

        rm->memory->supervisorMemory->VMPageTable[vm][page] = chosen;
    }
}