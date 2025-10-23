#include "parser.h"

void opCode(RM* rm, int nr, int* address, int opcode);

void register1(RM* rm, int nr, int* address, int r1);
void register2(RM* rm, int nr, int* address, bool* hasAddress, int r2);
void register3(RM* rm, int nr, int* address, int r3);
void register4(RM* rm, int nr, int* address, int r4);

void address1(RM* rm, int nr, int* address, bool* hasAddress, int value);

// // For testing: print bits of a byte
// void print_bits(uint8_t value) {
//     for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
//         printf("%d", (value >> i) & 1);
//         if (i % 8 == 0) printf(" ");
//     }
//     printf("\n");
// }

int parse(RM *rm, const char *filename, int nr) {

    if (filename == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }

    int phase = DATA;

    int* address = malloc(sizeof(int));
    if (address == NULL) {
        fclose(file);
        free(address);
        return 1;
    }
    
    *address = 0;

    bool* hasAddress = malloc(sizeof(bool));
    if (hasAddress == NULL) {
        fclose(file);
        free(hasAddress);
        return 1;
    }

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

        if(strncmp(codeStart, "$START", 6) == 0) {
            phase = CODE;
        } else if(phase == DATA) {
            int value = 0;

            int tempAddress = 0;

            if(sscanf(codeStart, "%x %x", &tempAddress, &value) != 2) {
                fclose(file);
                return 1;
            }

            if(tempAddress < 0 || tempAddress >= DATA_MEMORY * PAGE_SIZE * WORD_SIZE) {
                fclose(file);
                return 1; 
            }

            rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + tempAddress] = value;

        } else {
            *hasAddress = false;
            int o1, o2, o3, o4;

            // Aritmetines komandos
            if(strncmp(codeStart, "ADD", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x", &o1, &o2, &o3) == 3) {
                    opCode(rm, nr, address, ADDxyz);
                    register1(rm, nr, address, o1);
                    register2(rm, nr, address, hasAddress, o2);
                    register3(rm, nr, address, o3);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SUB", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x", &o1, &o2, &o3) == 3) {
                    opCode(rm, nr, address, SUBxyz);
                    register1(rm, nr, address, o1);
                    register2(rm, nr, address, hasAddress, o2);
                    register3(rm, nr, address, o3);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "MUL", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x R%x", &o1, &o2, &o3, &o4) == 4) {
                    opCode(rm, nr, address, MULxyzw);
                    register1(rm, nr, address, o1);
                    register2(rm, nr, address, hasAddress, o2);
                    register3(rm, nr, address, o3);
                    register4(rm, nr, address, o4);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "DIV", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x R%x", &o1, &o2, &o3, &o4) == 4) {
                    opCode(rm, nr, address, DIVxyzw);
                    register1(rm, nr, address, o1);
                    register2(rm, nr, address, hasAddress, o2);
                    register3(rm, nr, address, o3);
                    register4(rm, nr, address, o4);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Palyginimo komandos
            else if(strncmp(codeStart, "CMP", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x", &o1, &o2) == 2) {
                    opCode(rm, nr, address, CMPxy);
                    register1(rm, nr, address, o1);
                    register2(rm, nr, address, hasAddress, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Darbo su atmintimi komandos
            else if(strncmp(codeStart, "MR", 2) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    opCode(rm, nr, address, MRxyz);
                    address1(rm, nr, address, hasAddress, o1);
                    register2(rm, nr, address, hasAddress, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "MW", 2) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    opCode(rm, nr, address, MWxyz);
                    address1(rm, nr, address, hasAddress, o1);
                    register2(rm, nr, address, hasAddress, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SMR", 3) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    opCode(rm, nr, address, SMRxyz);
                    address1(rm, nr, address, hasAddress, o1);
                    register2(rm, nr, address, hasAddress, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SMW", 3) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    opCode(rm, nr, address, SMWxyz);
                    address1(rm, nr, address, hasAddress, o1);
                    register2(rm, nr, address, hasAddress, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "WAIT", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    opCode(rm, nr, address, WAIT);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SIGNAL", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    opCode(rm, nr, address, SIGNAL);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Valdymo komandos
            else if(strncmp(codeStart, "JMP", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JMPxy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JE", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JExy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JNE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JNExy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JG", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JGxy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JGE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JGExy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JL", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JLxy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JLE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JLExy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JC", 2) == 0) {
                if(sscanf(codeStart, "%*s   %x", &o1) == 1) {
                    opCode(rm, nr, address, JCxy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JNC", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, JNCxy);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "HALT", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    opCode(rm, nr, address, HALT);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Ivedimo/Išvedimo komandos
            else if(strncmp(codeStart, "DMAR", 4) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, DMARx);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "DMAW", 4) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    opCode(rm, nr, address, DMAWx);
                    address1(rm, nr, address, hasAddress, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else {
                fclose(file);
                return 1;

            }
        }
    }

    fclose(file);

    return 0;
}

void opCode(RM* rm, int nr, int* address, int opcode) {
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] = opcode << 3;
    (*address)++;
}

void register1(RM* rm, int nr, int* address, int r1) {
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 1] |= (r1 >> 1);
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] = (r1 & 0b00000001) << 7;
    (*address)++;
}

void register2(RM* rm, int nr, int* address, bool* hasAddress, int r2) {
    if (*hasAddress == false) {
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 1] |= (r2 << 3);
        return;
    } else {
        register3(rm, nr, address, r2);
    }
}

void register3(RM* rm, int nr, int* address, int r3) {
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 1] |= (r3 >> 1);
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] = (r3 & 0b00000001) << 7;
    (*address)++;
}

void register4(RM* rm, int nr, int* address, int r4) {
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 1] |= (r4 << 3);
}

void address1(RM* rm, int nr, int* address, bool* hasAddress, int value) {
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 1] |= (value >> 5);
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] = (value & 0b00011111) << 3;
    (*address)++;
    *hasAddress = true;
}