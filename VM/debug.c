#include "debug.h"

void dump_vm_memory(VM* vm, RM* rm);

void debug(RM* rm, VM* vm, Channel_device* channelDevice) {

    outputchannel(channelDevice, "0-run full program\n");
    outputchannel(channelDevice, "1-run 1 step of the program\n");
    outputchannel(channelDevice, "2-see register values\n");
    outputchannel(channelDevice, "3-see VM memory (paged)\n");

    inputchannel(channelDevice);
    uint8_t* action = rm->memory->supervisorMemory->buffer;

    char reg_val[256];

    switch (*action) {

        case RUN_FULL:
            outputchannel(channelDevice, "Debug: RUN_FULL\n");
            break;

        case STEP:
            outputchannel(channelDevice,"Debug: STEP\n");
            break;

        case REGISTER_VALUES: {
            outputchannel(channelDevice,"Debug: Registers\n");

            for (int i = 0; i < 16; i++) {
                snprintf(reg_val, sizeof(reg_val),
                        "R[%d] = %d", i, getRegister(vm, i));
                outputchannel(channelDevice, reg_val);
            }

            snprintf(reg_val, sizeof(reg_val),
                     "IC (virtual) = %u", getIc(vm));
            outputchannel(channelDevice, reg_val);

            snprintf(reg_val, sizeof(reg_val),
                     "FR = %d", getFr(vm));
            outputchannel(channelDevice, reg_val);

            break;
        }

        case VM_MEM: {
            outputchannel(channelDevice, "Debug: VM memory (paged view)\n");

            char msg[256];

            uint16_t ic = getIc(vm);

            int page = ic / PAGE_SIZE;
            int off  = ic % PAGE_SIZE;

            uint8_t* physPage =
                rm->pagingDevice->getAddressFromPage(rm->pagingDevice, vm->id, page);

            snprintf(msg, sizeof(msg),
                     "IC virtual: %u  -> Physical page %d offset %d\n",
                     ic, page, off);
            outputchannel(channelDevice, msg);

            snprintf(msg, sizeof(msg),
                     "=== CURRENT PAGE DUMP (physical frame %d) ===",
                     page);
            outputchannel(channelDevice, msg);

            for (int i = 0; i < PAGE_SIZE; i += 16) {
                snprintf(msg, sizeof(msg), "%04x: ", i);
                outputchannel(channelDevice, msg);

                for (int j = 0; j < 16; j++) {
                    snprintf(msg, sizeof(msg), "%02x ", physPage[i + j]);
                    outputchannel(channelDevice, msg);
                }
                outputchannel(channelDevice, "\n");
            }

            // full dump
            dump_vm_memory(vm, rm);
            break;
        }
    }
}

void dump_vm_memory(VM* vm, RM* rm) {
    printf("=== FULL VM MEMORY DUMP ===\n");

    for (uint16_t vaddr = 0; vaddr < TOTAL_MEMORY_SIZE; vaddr += 16) {

        uint16_t physRow[16];

        for (int i = 0; i < 16; i++) {
            int page = (vaddr + i) / PAGE_SIZE;
            int off  = (vaddr + i) % PAGE_SIZE;

            uint8_t* physPage =
                rm->pagingDevice->getAddressFromPage(rm->pagingDevice, vm->id, page);

            physRow[i] = physPage[off];
        }

        printf("%04x: ", vaddr);
        for (int i = 0; i < 16; i++)
            printf("%02x ", physRow[i]);
        printf("\n");
    }
}
