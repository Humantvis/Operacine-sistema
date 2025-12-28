#include "processList.h"

void initProcessList(ProcessList* list) {
    list->count = 0;
    for (int i = 0; i < MAX_PROCESESSES; i++) {
        list->items[i] = NULL;
    }
}

void insertProcess(ProcessList* list, Process* p) {
    if (!list || !p) return;

    for (int i = 0; i < list->count; i++) {
        if (list->items[i] == p) return; // already in queue
    }

    list->items[list->count++] = p;
}

Process* popProcess(ProcessList* list) {
    if (list == NULL || list->count == 0) {
        return NULL;
    }

    Process* p = list->items[0];

    // shift left
    for (int i = 1; i < list->count; i++) {
        list->items[i - 1] = list->items[i];
    }

    list->count--;
    list->items[list->count] = NULL;

    return p;
}

bool removeProcess(ProcessList* list, Process* p) {
    if (list == NULL || p == NULL) return false;

    int idx = -1;
    for (int i = 0; i < list->count; i++) {
        if (list->items[i] == p) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return false;

    for (int i = idx + 1; i < list->count; i++) {
        list->items[i - 1] = list->items[i];
    }

    list->count--;
    list->items[list->count] = NULL;
    return true;
}

bool containsProcess(ProcessList* list, Process* p) {
    if (list == NULL || p == NULL) return false;

    for (int i = 0; i < list->count; i++) {
        if (list->items[i] == p) return true;
    }
    return false;
}