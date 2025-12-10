#include "pagingDevice.h"

#include "../RM/supervisorMemory.h"

void initPagingDevice(PagingDevice* device, SupervisorMemory* supervisor) {
    device->supervisor = supervisor;
    device->getAddressFromPage = getAddressFromPage;
}

uint8_t* getAddressFromPage(PagingDevice* device, int vmID, uint8_t page) {
    return device->supervisor->VMPageTable[vmID][page];
}