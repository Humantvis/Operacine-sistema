#include "debug.h"
void debug(RM* rm, VM* vm, Channel_device* channelDevice) {

    outputchannel(channelDevice, "0-run full program\n");
    outputchannel(channelDevice, "1-run 1 step of the program\n");
    outputchannel(channelDevice, "2-see register values\n");
    outputchannel(channelDevice, "3-see VM memory\n");

    inputchannel(channelDevice);
    uint8_t* action = rm->cpu->buffer;

    char reg_val[256];
    switch (*action) {
        case RUN_FULL: {
            outputchannel(channelDevice, "Debug: RUN_FULL\n");

            break;
        }
        case STEP: {
            outputchannel(channelDevice,"Debug: STEP\n");
            break;
        }
        case REGISTER_VALUES: {
            outputchannel(channelDevice,"Debug: Registers\n");
            for (int i=0; i<16; i++)
            {
                snprintf(reg_val, sizeof(reg_val),
                "R[%d] = %d", i, getRegister(vm, i));
                outputchannel(channelDevice,reg_val);
            }
                snprintf(reg_val, sizeof(reg_val),
                "ic = %llx", getIc(vm));
                outputchannel(channelDevice,reg_val);
                snprintf(reg_val, sizeof(reg_val),
                "fr = %d", getFr(vm));
                outputchannel(channelDevice,reg_val);
            break;
        }
        case VM_MEM: {
            outputchannel(channelDevice, "Debug: VM memory\n");

            char msg[256];

            uint8_t* addr = vm->memory->codeMemory + ((getIc(vm) - vm->memory->codeMemory) / PAGE_SIZE) * PAGE_SIZE;

            snprintf(msg, sizeof(msg), "%llx:\n", (void*)addr);

            outputchannel(channelDevice, msg);

            for (int w = 0; w < PAGE_WORDS; w++) {
                snprintf(msg, sizeof(msg), "%x %x %x %x\n", *(addr + WORD_SIZE * w), *(addr + WORD_SIZE * w + 1), *(addr + WORD_SIZE * w + 2), *(addr + WORD_SIZE * w + 3));
                outputchannel(channelDevice, msg);
            }

            outputchannel(channelDevice, "\n");

            break;
        }
    }
}