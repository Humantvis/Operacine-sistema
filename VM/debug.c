#include "debug.h"
#include <stdint.h>

void dump_vm_memory(VM* vm, RM* rm) {
    char msg[256];

    outputchannel(rm->channelDevice, "=== FULL VM MEMORY DUMP (VIRTUAL → PAGED) ===\n");

    for (uint16_t vaddr = 0; vaddr < TOTAL_MEMORY_SIZE; vaddr += 16) {

        // Print virtual address header
        snprintf(msg, sizeof(msg), "%04x: ", vaddr);
        outputchannel(rm->channelDevice, msg);

        for (int i = 0; i < 16; i++) {

            uint16_t curr = vaddr + i;

            int page   = curr / PAGE_SIZE;
            int offset = curr % PAGE_SIZE;

            uint8_t* physPage =
                rm->pagingDevice->getAddressFromPage(
                    rm->pagingDevice,
                    vm->id,
                    page
                );

            uint8_t value = physPage[offset];

            snprintf(msg, sizeof(msg), "%02x ", value);
            outputchannel(rm->channelDevice, msg);
        }

        outputchannel(rm->channelDevice, "\n");
    }

    outputchannel(rm->channelDevice, "=== END OF VM MEMORY DUMP ===\n");
}

uint8_t debug(Kernel* kernel, VM* vm, Channel_device* channelDevice) {

    outputchannel(channelDevice, "0-run full program\n");
    outputchannel(channelDevice, "1-run 1 step of the program\n");
    outputchannel(channelDevice, "2-see register values\n");
    outputchannel(channelDevice, "3-see VM memory (paged)\n");

    inputchannel(channelDevice);

    uint8_t action = kernel->rm->memory->supervisorMemory->buffer[0];
    kernel->debugMode = action;

    switch (action) {

        case REGISTER_VALUES:
            for (int i = 0; i < 16; i++) {
                char buf[64];
                snprintf(buf, sizeof(buf), "R[%d] = %d", i, getRegister(vm, i));
                outputchannel(channelDevice, buf);
            }
            break;

        case VM_MEM:
            dump_vm_memory(vm, kernel->rm);
            break;

        case RUN_FULL:
        case STEP:
        default:
            break;
    }

    return action;
}