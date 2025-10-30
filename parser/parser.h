#include "../RM/rm.h"
#include "../VM/vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    DATA,
    CODE
};

int parse(RM *rm, const char *filename, int nr);