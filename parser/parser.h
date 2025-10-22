#include "../RM/rm.h"
#include "../VM/vm.h"

enum {
    DATA,
    CODE
};

int parse(RM *rm, const char *filename, int nr);