#include "rm.h"
#include "vm.h"
#include "externalMemory.h"
#include "parser.h"
#include "debug.h"

int main() {
    RM rm;
    initialize_RM(&rm);

    initChannelDevice(rm.channelDevice, rm.cpu, rm.memory);

    free_RM(&rm);

    return 0;
}