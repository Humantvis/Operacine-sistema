#include "resourceManager.h"
#include <stdlib.h>
#include <stdio.h>

static void list_push(ProcessList* list, VM* vm) {
    if (!list || !vm) {
        return;
    }

    list->items[list->count++] = vm;
}

static VM* list_pop_front(ProcessList* list) {
    if (!list || list->count <= 0) {
        return NULL;
    }

    VM* first = list->items[0];
    for (int i = 1; i < list->count; i++) {
        list->items[i - 1] = list->items[i];
    }
    list->count--;
    return first;
}


static Resource* getRes(ResourceManager* rm, ResourceType type) {
    if (!rm) {
        return NULL;
    }

    if (type < 0 || type >= RES_COUNT) {
        return NULL;
    }

    return &rm->resources[type];
}

void initResourceManager(ResourceManager* rm) {
    if (!rm) {
        return;
    }

    for (int i = 0; i < RES_COUNT; i++) {
        rm->resources[i].busy = false;
        rm->resources[i].owner = NULL;
        rm->resources[i].waitQ = malloc(sizeof(ProcessList));
        
        initList(rm->resources[i].waitQ, T_USER, rm->kernel);
    }

    rm->resources[RES_CPU].name = "CPU";
    rm->resources[RES_CHANNEL].name = "ChannelDevice";
    rm->resources[RES_PAGING].name = "PagingDevice";
    rm->resources[RES_EXTERNAL_MEMORY].name = "ExternalMemory";
    rm->resources[RES_INPUT].name = "Input";
}

void destroyResourceManager(ResourceManager* rm) {
    if (!rm) {
        return;
    }

    for (int i = 0; i < RES_COUNT; i++) {
        if (rm->resources[i].waitQ) {
            free(rm->resources[i].waitQ);
            rm->resources[i].waitQ = NULL;
        }
        rm->resources[i].busy = false;
        rm->resources[i].owner = NULL;
        rm->resources[i].name = NULL;
    }

    rm->kernel = NULL;
}

bool requestResource(ResourceManager* rm, ResourceType type, VM* vm) {
    Resource* r = getRes(rm, type);
    if (!r || !vm) {
        return false;
    }

    if (!r->busy) {
        r->busy = true;
        r->owner = vm;
        return true;
    }

    if (r->owner == vm) {
        return true;
    }

    list_push(r->waitQ, vm);
    return false;
}

void releaseResource(ResourceManager* rm, ResourceType type, VM* owner) {
    Resource* r = getRes(rm, type);
    if (!r) {
        return;
    }

    if (!r->busy) {
        return;
    }

    if (r->owner != owner) {
        return;
    }

    VM* next = list_pop_front(r->waitQ);

    if (next) {
        r->busy = true;
        r->owner = next;

        if (rm->kernel && rm->kernel->readyUser) {
            rm->kernel->readyUser->items[rm->kernel->readyUser->count++] = next;
        }
    } else {
        r->busy = false;
        r->owner = NULL;
    }
}
