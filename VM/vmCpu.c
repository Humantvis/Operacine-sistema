#include "vmCpu.h"
void initVMCPU(VM_CPU* cpu, VM_Memory* memory) {
    for (int i = 0; i < REGISTERS; i++) {
        cpu->r[i] = 0;
    }
    
    cpu->ic = memory->codeMemory;

    cpu->fr = 0;
}