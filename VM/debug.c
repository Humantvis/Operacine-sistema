#include "debug.h"
void debug(RM* rm, VM* vm, Channel_device* channelDevice) {

    outputchannel(channelDevice, "0-run full program\n");
    outputchannel(channelDevice, "1-run 1 step of the program\n");
    outputchannel(channelDevice, "2-see register values\n");
    outputchannel(channelDevice, "3-see RM memory\n");
    outputchannel(channelDevice, "4-see VM memory\n");

    inputchannel(channelDevice);
    uint8_t action = rm->cpu->buffer;

    char reg_val[256];
    switch (action) {
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
                "R[%d] = %d", i, rm->cpu->r[i]);
                outputchannel(channelDevice,reg_val);
            }
                snprintf(reg_val, sizeof(reg_val),
                "ic = %p", rm->cpu->ic);
                outputchannel(channelDevice,reg_val);
                snprintf(reg_val, sizeof(reg_val),
                "fr = %d", rm->cpu->fr);
                outputchannel(channelDevice,reg_val);
            break;
        }
        case RM_MEM: {
            outputchannel(channelDevice, "=== RM MEMORY DUMP ===\n");
            char msg[256];

            struct {
                const char* name;
                uint8_t* base;
                size_t size;
            } regions[] = {
                { "User Memory", rm->memory->userMemory, USER_MEMORY_SIZE * PAGE_SIZE * WORD_SIZE },
                { "Supervisor Memory", rm->memory->supervizorMemory, SUPERVIZOR_MEMORY_SIZE * PAGE_SIZE * WORD_SIZE },
                { "Shared Memory", rm->memory->sharedMemory, SHARED_MEMORY * PAGE_SIZE * WORD_SIZE }
            };

            for (int r = 0; r < 3; r++) {
                snprintf(msg, sizeof(msg), "--- %s (%zu bytes) ---\n", regions[r].name, regions[r].size);
                outputchannel(channelDevice, msg);

                uint8_t* base = regions[r].base;
                size_t totalSize = regions[r].size;

                for (size_t addr = 0; addr < totalSize; addr += 64) { // 16 words per row
                    snprintf(msg, sizeof(msg), "0x%04lX: ", addr);
                    outputchannel(channelDevice, msg);

                    for (int w = 0; w < 16 && (addr + w * 4) < totalSize; w++) {
                        size_t i = addr + w * 4;
                        snprintf(msg, sizeof(msg),
                                "%02X%02X%02X%02X ",
                                base[i],
                                base[i + 1],
                                base[i + 2],
                                base[i + 3]);
                        outputchannel(channelDevice, msg);
                    }
                    outputchannel(channelDevice, "\n");
                }
            }

            break;
        }

        case VM_MEM: {
            outputchannel(channelDevice, "=== VM MEMORY DUMP ===\n");

            char msg[256];

            // we assume VM memory is contiguous inside RM memory
            uint8_t *base = vm->memory->dataMemory; 
            if (base == NULL) {
                outputchannel(channelDevice, "(null vm->memory->dataMemory)\n");
                break;
            }

            // compute total size of all VM memory
            size_t totalSize = TOTAL_MEMORY_SIZE;

            snprintf(msg, sizeof(msg), "Base address: %p, total size: %zu bytes\n", (void*)base, totalSize);
            outputchannel(channelDevice, msg);

            // print 16x16 words (1 word = 4 bytes)
            for (size_t addr = 0; addr < totalSize; addr += 64) { // 16 words per row
                snprintf(msg, sizeof(msg), "0x%04lX: ", addr);
                outputchannel(channelDevice, msg);

                for (int w = 0; w < 16 && (addr + w * 4) < totalSize; w++) {
                    size_t i = addr + w * 4;
                    snprintf(msg, sizeof(msg),
                            "%02X%02X%02X%02X ",
                            base[i],
                            base[i + 1],
                            base[i + 2],
                            base[i + 3]);
                    outputchannel(channelDevice, msg);
                }

                outputchannel(channelDevice, "\n");
            }

            break;
        }
            }
        }