#include "processList.h"
void initList(ProcessList* list, int type, Kernel* kernel)
{
    list->type = type;
    list -> kernel = kernel;
    list->count = 0;
    memset(list->items, 0, sizeof(list->items));
}
void addItem (ProcessList* list, VM* vm)
{
    if (list->count < MAX_PROCESESSES)
    {
        list->items[list->count] = vm;
        list->count++;
    }
}