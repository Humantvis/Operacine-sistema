#include "../defines.h"

typedef struct SavedRegisters {
	uint8_t* r[REGISTERS];
    uint16_t* ic;
    int* offset;
    uint8_t* fr;
} SavedRegisters;