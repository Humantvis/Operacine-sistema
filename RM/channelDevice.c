#include "channelDevice.h"

void initChannelDevice(Channel_device* channelDevice, RM_CPU* cpu, RM_Memory* memory) {

    channelDevice->sb = 0;
    channelDevice->db = 0;
    channelDevice->st = 0;
    channelDevice->dt = 0;
    channelDevice->memory = memory;
    channelDevice->cpu = cpu;

}
void resetChannelDevice(Channel_device* channelDevice) {
    channelDevice->sb = 0;
    channelDevice->db = 0;
    channelDevice->st = 0;
    channelDevice->dt = 0;
}
int callChannelDevice(Channel_device* channelDevice) {
    RM_Memory* mem = channelDevice->memory;
    RM_CPU* cpu = channelDevice->cpu;
    uint8_t* src = 0;
    uint8_t* dst = 0;

    switch (channelDevice->st) {
        case User_Memory:
            src = &mem->userMemory[channelDevice->sb];
            break;
        case Supervizor_Memory:
            src = &mem->supervizorMemory[channelDevice->sb];
            break;
        case Shared_Memory:
            src = &mem->sharedMemory[channelDevice->sb];
            break;
        case CPU:
            if (channelDevice->sb < REGISTERS)
                src = &cpu->r[channelDevice->sb];
            break;
        case IO_Device:
            return -3; 
        default:
            return -4; 
    }

    switch (channelDevice->dt) {
        case User_Memory:
            dst = &mem->userMemory[channelDevice->db];
            break;
        case Supervizor_Memory:
            dst = &mem->supervizorMemory[channelDevice->db];
            break;
        case Shared_Memory:
            dst = &mem->sharedMemory[channelDevice->db];
            break;
        case CPU:
            if (channelDevice->db < REGISTERS);
            break;
        case IO_Device:
            return -3;
        default:
            return -4; 
    }


    *dst = *src;

    return 0; 
}