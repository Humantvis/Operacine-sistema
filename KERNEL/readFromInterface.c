#include <stdio.h>

#include "../defines.h"

#include "readFromInterface.h"

void readFromInterface(char **output) {
    static char buffer[BUFFER_SIZE];

    printf("> ");
    fflush(stdout);

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        *output = NULL;
        return;
    }

    buffer[strcspn(buffer, "\n")] = 0;
    *output = buffer;
}