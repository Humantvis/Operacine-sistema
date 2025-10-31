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
    printf("Input:\n");

    // Clear buffer first
    for (int i = 0; i < BUFFER_SIZE; i++) {
        channelDevice->cpu->buffer[i] = 0;
    }

    char line[512];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;  // failed to read line
    }

    // Remove trailing newline if present
    line[strcspn(line, "\n")] = '\0';

    char* token = strtok(line, " ");
    int i = 0;

    while (token != NULL && i < BUFFER_SIZE) {
        int value = atoi(token);  // convert token to integer

        if (value < 0) value = 0;
        if (value > 255) value = 255;

        channelDevice->cpu->buffer[i] = (uint8_t)value;
        i++;
        token = strtok(NULL, " ");
    }

    return 0;  // success
}

int outputchannel(Channel_device* channelDevice, const char* data) {
    
    printf(" %s\n", data);

} 