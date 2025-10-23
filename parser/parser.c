#include "parser.h"

void writeOpCode(RM* rm, int nr, int* address, int* offset, int opcode);

void writeReg(RM* rm, int nr, int* address, int* offset, int r1);

void writeAddress(RM* rm, int nr, int* address, int* offset, int value);

// For testing: print bits of a byte
void print_bits(uint8_t value) {
    for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

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

    int* offset = malloc(sizeof(int));
    if (offset == NULL) {
        fclose(file);
        free(address);
        return 1;
    }

    *offset = 0;

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
            int o1, o2, o3, o4;

            // Aritmetines komandos
            if(strncmp(codeStart, "ADD", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x", &o1, &o2, &o3) == 3) {
                    writeOpCode(rm, nr, address, offset, ADDxyz);
                    writeReg(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                    writeReg(rm, nr, address, offset, o3);
                    print_bits(rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 2]);
                    print_bits(rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address - 1]);
                    print_bits(rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address]);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SUB", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x", &o1, &o2, &o3) == 3) {
                    writeOpCode(rm, nr, address, offset, SUBxyz);
                    writeReg(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                    writeReg(rm, nr, address, offset, o3);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "MUL", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x R%x", &o1, &o2, &o3, &o4) == 4) {
                    writeOpCode(rm, nr, address, offset, MULxyzw);
                    writeReg(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                    writeReg(rm, nr, address, offset, o3);
                    writeReg(rm, nr, address, offset, o4);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "DIV", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x R%x", &o1, &o2, &o3, &o4) == 4) {
                    writeOpCode(rm, nr, address, offset, DIVxyzw);
                    writeReg(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                    writeReg(rm, nr, address, offset, o3);
                    writeReg(rm, nr, address, offset, o4);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Palyginimo komandos
            else if(strncmp(codeStart, "CMP", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x", &o1, &o2) == 2) {
                    writeOpCode(rm, nr, address, offset, CMPxy);
                    writeReg(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Darbo su atmintimi komandos
            else if(strncmp(codeStart, "MR", 2) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(rm, nr, address, offset, MRxyz);
                    writeAddress(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "MW", 2) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(rm, nr, address, offset, MWxyz);
                    writeAddress(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SMR", 3) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(rm, nr, address, offset, SMRxyz);
                    writeAddress(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SMW", 3) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(rm, nr, address, offset, SMWxyz);
                    writeAddress(rm, nr, address, offset, o1);
                    writeReg(rm, nr, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "WAIT", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    writeOpCode(rm, nr, address, offset, WAIT);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SIGNAL", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    writeOpCode(rm, nr, address, offset, SIGNAL);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Valdymo komandos
            else if(strncmp(codeStart, "JMP", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JMPxy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JE", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JExy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JNE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JNExy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JG", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JGxy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JGE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JGExy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JL", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JLxy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JLE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JLExy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JC", 2) == 0) {
                if(sscanf(codeStart, "%*s   %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JCxy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JNC", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, JNCxy);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "HALT", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    writeOpCode(rm, nr, address, offset, HALT);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Ivedimo/Išvedimo komandos
            else if(strncmp(codeStart, "DMAR", 4) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, DMARx);
                    writeAddress(rm, nr, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "DMAW", 4) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(rm, nr, address, offset, DMAWx);
                    writeAddress(rm, nr, address, offset, o1);
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

void writeOpCode(RM* rm, int nr, int* address, int* offset, int opcode) {
    if(*offset <= 3) {
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] |= (opcode << (3 - *offset));
    } else {
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] |= (opcode >> (*offset - 3));
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address + 1] = (opcode << (11 - *offset));
    }

    *offset += 5;
    if(*offset >= 8) {
        *offset = *offset % 8;
        (*address)++;
    }
}

void writeReg(RM* rm, int nr, int* address, int* offset, int r) {
    if(*offset <= 4) {
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] |= (r << (4 - *offset));
    } else {
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] |= (r >> (*offset - 4));
        rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address + 1] = (r << (12 - *offset));
    }

    *offset += 4;
    if(*offset >= 8) {
        *offset = *offset % 8;
        (*address)++;
    }
}

void writeAddress(RM* rm, int nr, int* address, int* offset, int value) {
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address] |= (value >> *offset);
    rm->memory->userMemory[nr * TOTAL_MEMORY_SIZE + DATA_MEMORY + *address + 1] = (value << (8 - *offset));

    (*address)++;
}