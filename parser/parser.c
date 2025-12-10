#include "parser.h"

void writeOpCode(int programID, uint8_t** address, uint8_t* offset, int opcode);

void writeReg(int programID, uint8_t** address, uint8_t* offset, int r1);

void writeAddress(int programID, uint8_t** address, uint8_t* offset, int value);

void writeJmpAddress(int programID, uint8_t** address, uint8_t* offset, int value);


// // For testing: print bits of a byte
// void print_bits(uint8_t value) {
//     for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
//         printf("%d", (value >> i) & 1);
//         if (i % 8 == 0) printf(" ");
//     }
//     printf("\n");
// }

int parse(ExternalMemory* externalMemory, const char *filename, int programID) {

    if (filename == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }

    int phase = DATA;

    uint8_t** address = malloc(sizeof(uint8_t*));
    if (address == NULL) {
        fclose(file);
        free(address);
        return 1;
    }
    
    *address = externalMemory->memory + programID * TOTAL_MEMORY_SIZE + DATA_MEMORY * PAGE_WORDS * WORD_SIZE;

    uint8_t* offset = malloc(sizeof(int));
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
        } else if(strncmp(codeStart, "$END", 4) == 0) {
            fclose(file);

            return 0;
        } else if(phase == DATA) {
            int value = 0;

            int tempAddress = 0;

            if(sscanf(codeStart, "%x %x", &tempAddress, &value) != 2) {
                fclose(file);
                return 1;
            }

            if(tempAddress < 0 || tempAddress >= DATA_MEMORY * PAGE_SIZE) {
                fclose(file);
                return 1; 
            }

            externalMemory->memory[programID * TOTAL_MEMORY_SIZE + tempAddress] = value;

        } else {
            int o1, o2, o3, o4;

            uint8_t jumpLocation;
            if(strchr(codeStart, ':') && sscanf(codeStart, "%x:", &jumpLocation) == 1) {
                if(jumpLocation >= 0 && jumpLocation < 16) {
                    uint64_t addr_val = (uint64_t)(uintptr_t)*address;
                    size_t base_index = programID * TOTAL_MEMORY_SIZE + (DATA_MEMORY + CODE_MEMORY) * PAGE_WORDS * WORD_SIZE - jumpLocation * 9;

                    for (int i = 0; i < 8; i++) {
                        *(externalMemory->memory + base_index + i) = (addr_val >> (i * 8)) & 0b11111111;
                    }

                    *(externalMemory->memory + base_index + 8) = *offset;
                } else {
                    printf("jump can only be from 0 to 15");
                }
            }
            // Aritmetines komandos
            else if(strncmp(codeStart, "ADD", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x", &o1, &o2, &o3) == 3) {
                    writeOpCode(programID, address, offset, ADDxyz);
                    writeReg(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                    writeReg(programID, address, offset, o3);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SUB", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x", &o1, &o2, &o3) == 3) {
                    writeOpCode(programID, address, offset, SUBxyz);
                    writeReg(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                    writeReg(programID, address, offset, o3);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "MUL", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x R%x", &o1, &o2, &o3, &o4) == 4) {
                    writeOpCode(programID, address, offset, MULxyzw);
                    writeReg(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                    writeReg(programID, address, offset, o3);
                    writeReg(programID, address, offset, o4);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "DIV", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x R%x R%x", &o1, &o2, &o3, &o4) == 4) {
                    writeOpCode(programID, address, offset, DIVxyzw);
                    writeReg(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                    writeReg(programID, address, offset, o3);
                    writeReg(programID, address, offset, o4);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Palyginimo komandos
            else if(strncmp(codeStart, "CMP", 3) == 0) {
                if(sscanf(codeStart, "%*s R%x R%x", &o1, &o2) == 2) {
                    writeOpCode(programID, address, offset, CMPxy);
                    writeReg(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Darbo su atmintimi komandos
            else if(strncmp(codeStart, "MR", 2) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(programID, address, offset, MRxyz);
                    writeAddress(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "MW", 2) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(programID, address, offset, MWxyz);
                    writeAddress(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SMR", 3) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(programID, address, offset, SMRxyz);
                    writeAddress(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SMW", 3) == 0) {
                if(sscanf(codeStart, "%*s %x R%x", &o1, &o2) == 2) {
                    writeOpCode(programID, address, offset, SMWxyz);
                    writeAddress(programID, address, offset, o1);
                    writeReg(programID, address, offset, o2);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "WAIT", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    writeOpCode(programID, address, offset, WAIT);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "SIGNAL", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    writeOpCode(programID, address, offset, SIGNAL);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Valdymo komandos
            else if(strncmp(codeStart, "JMP", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JMPxy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JE", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JExy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JNE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JNExy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JG", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JGxy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JGE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JGExy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JL", 2) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JLxy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JLE", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JLExy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JC", 2) == 0) {
                if(sscanf(codeStart, "%*s   %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JCxy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "JNC", 3) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, JNCxy);
                    writeJmpAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "HALT", 4) == 0) {
                if(sscanf(codeStart, "%*s") == 0) {
                    writeOpCode(programID, address, offset, HALT);
                } else {
                    fclose(file);
                    return 1;
                }
            }
            // Ivedimo/Išvedimo komandos
            else if(strncmp(codeStart, "DMAR", 4) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, DMARx);
                    writeAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else if(strncmp(codeStart, "DMAW", 4) == 0) {
                if(sscanf(codeStart, "%*s %x", &o1) == 1) {
                    writeOpCode(programID, address, offset, DMAWx);
                    writeAddress(programID, address, offset, o1);
                } else {
                    fclose(file);
                    return 1;
                }
            } else {
                fclose(file);
                return 1;

            }
            // // For testing: prints out the last 4 recorded bytes after each new command
            // print_bits(*(*address - 3));
            // print_bits(*(*address - 2));
            // print_bits(*(*address - 1));
            // print_bits(**address);
            // printf("\n");
        }
    }

    fclose(file);

    return -1;
}

void writeOpCode(int programID, uint8_t** address, uint8_t* offset, int opcode) {
    if(*offset <= 3) {
        **address |= (opcode << (3 - *offset));
    } else {
        **address |= (opcode >> (*offset - 3));
        *(*address + 1) = (opcode << (11 - *offset));
    }

    *offset += 5;
    if(*offset >= 8) {
        *offset = *offset % 8;
        (*address)++;
    }
}

void writeReg(int programID, uint8_t** address, uint8_t* offset, int r) {
    if(*offset <= 4) {
        **address |= (r << (4 - *offset));
    } else {
        **address |= (r >> (*offset - 4));
        *(*address + 1) = (r << (12 - *offset));
    }

    *offset += 4;
    if(*offset >= 8) {
        *offset = *offset % 8;
        (*address)++;
    }
}

void writeAddress(int programID, uint8_t** address, uint8_t* offset, int value) {
    **address |= (value >> *offset);
    *(*address + 1) = (value << (8 - *offset));

    (*address)++;
}

void writeJmpAddress(int programID, uint8_t** address, uint8_t* offset, int value) {
    if(*offset <= 4) {
        **address |= (value << (4 - *offset));
    } else {
        **address |= (value >> (*offset - 4));
        *(*address + 1) = (value << (12 - *offset));
    }

    *offset += 4;
    if(*offset >= 8) {
        *offset = *offset % 8;
        (*address)++;
    }
}