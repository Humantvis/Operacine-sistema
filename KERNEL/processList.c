#include "processList.h"
void initList(ProcessList* list, int type, Kernel* kernel)
{
    list->type = type;
    list -> kernel = kernel;
    list->count = 0;
    memset(list->items, 0, sizeof(list->items));
}
void addItem (ProcessList* list, Process* process)
{
    if (list->count < MAX_PROCESESSES)
    {
        list->items[list->count] = process;
        list->count++;
    }
}
void jobToSwap(Process* process)
{
    if (process->priority == T_SYSTEM) {
        process->currentList = process->kernel->readySystem;
    } else {
        process->currentList = process->kernel->readyUser;
    }
    addItem(process->currentList, process);
}