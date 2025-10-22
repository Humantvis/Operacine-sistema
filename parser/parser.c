#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int parse(RM *rm, const char *filename, int nr) {

    if (filename == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }

    int phase = DATA;

    int address = 0;

    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == ';' || line[0] == '\n') {
            continue;  
        }

        char *codeStart = line;
        while (*codeStart == ' ' || *codeStart == '\t') {
            codeStart++;
        }

        char *codeEnd = codeStart;
        while (*codeEnd != '\n' && *codeEnd != '\0') {
            codeEnd++;
        }
        *codeEnd = '\0';

        if(codeStart[0] == '$' && codeStart[1] == 'S' && codeStart[2] == 'T' && codeStart[3] == 'A' && codeStart[4] == 'R' && codeStart[5] == 'T') {
            phase = CODE;
        }
        
        if(phase == DATA) {
            int value = 0;

            for (int i = 3; i < (int)(codeEnd - codeStart); i++) {
                value = value * 16 + (codeStart[i] - '0');
            }

            int address = (codeStart[0] - '0') * 16 + (codeStart[1] - '0');

            if(address < 0 || address >= DATA_MEMORY * PAGE_SIZE * WORD_SIZE) {
                fclose(file);
                return 1; 
            }

            rm->memory->userMemory[address] = value;

        } else {
            if(codeStart[0] == 'A' && codeStart[1] == 'D' && codeStart[2] == 'D') {
                int r1, r2, r3;

                rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + address] = ADDxyz << 3;

                if(sscanf(codeStart, "%*s R%d R%d R%d", &r1, &r2, &r3) == 3) {
                    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + address] |= (r1 >> 1);
                    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + address + 1] = (r1 & 0b00000001) << 7 | (r2 << 3) | (r3 >> 1);
                    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + address + 2] = (r3 & 0b00000001) << 7;
                } else {
                    fclose(file);
                    return 1;
                }

                address += 3;
            }

        }
    }

    fclose(file);

    return 0;
}