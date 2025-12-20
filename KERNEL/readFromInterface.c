#include <stdio.h>

#include "defines.h"

#include "readFromInterface.h"

char* readFromInterface(void) {
    static char buffer[BUFFER_SIZE];

    printf("> ");
    fflush(stdout);

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = 0;
    return buffer;
}