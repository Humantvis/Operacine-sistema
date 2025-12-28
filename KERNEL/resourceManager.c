#include "resourceManager.h"
#include "process.h"
#include "processList.h"
#include <stdlib.h>
#include <stdio.h>

/* ---------- helpers for ProcessList ---------- */

static void list_push(ProcessList* list, Process* p) {
    if (!list || !p) return;
    insertProcess(list, p);
}

static Process* list_pop_front(ProcessList* list) {
    if (!list || list->count == 0) return NULL;
    return popProcess(list);
}

/* ---------- internal helper ---------- */

static Resource* getRes(ResourceManager* rm, ResourceType type) {
    if (!rm || type < 0 || type >= RES_COUNT) {
        return NULL;
    }
    return &rm->resources[type];
}

/* ---------- init / destroy ---------- */

void initResourceManager(ResourceManager* rm) {
    if (!rm) return;

    for (int i = 0; i < RES_COUNT; i++) {
        rm->resources[i].busy = false;
        rm->resources[i].owner = NULL;

        rm->resources[i].waitQ = malloc(sizeof(ProcessList));
        initProcessList(rm->resources[i].waitQ);
    }

    rm->resources[RES_CPU].name = "CPU";
    rm->resources[RES_CHANNEL].name = "ChannelDevice";
    rm->resources[RES_PAGING].name = "PagingDevice";
    rm->resources[RES_EXTERNAL_MEMORY].name = "ExternalMemory";
    rm->resources[RES_INPUT].name = "Input";
}

void destroyResourceManager(ResourceManager* rm) {
    if (!rm) return;

    for (int i = 0; i < RES_COUNT; i++) {
        free(rm->resources[i].waitQ);
        rm->resources[i].waitQ = NULL;
        rm->resources[i].busy = false;
        rm->resources[i].owner = NULL;
        rm->resources[i].name = NULL;
    }

    rm->kernel = NULL;
}

/* ---------- resource operations ---------- */

bool requestResource(ResourceManager* rm, ResourceType type, Process* p) {
    Resource* r = getRes(rm, type);
    if (!r || !p) return false;

    if (!r->busy) {
        r->busy = true;
        r->owner = p;
        return true;
    }

    if (r->owner == p) {
        return true;
    }

    changeState(p, BLOCKED);
    list_push(r->waitQ, p);
    return false;
}

void releaseResource(ResourceManager* rm, ResourceType type, Process* owner) {
    Resource* r = getRes(rm, type);
    if (!r || r->owner != owner) return;

    Process* next = list_pop_front(r->waitQ);

    if (next) {
        r->busy = true;
        r->owner = next;

        changeState(next, READY);
        insertProcess(rm->kernel->readyUser, next);
    } else {
        r->busy = false;
        r->owner = NULL;
    }
}