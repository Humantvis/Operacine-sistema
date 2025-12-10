#pragma once

#include <stdint.h>
#include "../defines.h"

struct SupervisorMemory;

typedef struct PagingDevice {
    struct SupervisorMemory* supervisor;
    
    uint8_t* (*getAddressFromPage)(struct PagingDevice* device, int vmID, uint8_t page);
} PagingDevice;


void initPagingDevice(PagingDevice* device, struct SupervisorMemory* supervisor);

uint8_t* getAddressFromPage(PagingDevice* device, int vmID, uint8_t page);