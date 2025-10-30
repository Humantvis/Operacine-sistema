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
int inputchannel(Channel_device* channelDevice) {
    uint8_t userInput;
    if (scanf("%hhu", &userInput) != 1) {
        return -1;  // failed to read input
    }
        channelDevice->cpu->buffer = userInput;  // store input directly in CPU buffer, segmentation fault here
    return 0;  // success
}
int outputchannel(Channel_device* channelDevice, const char* data) {
    
    printf(" %s\n", data);

} 