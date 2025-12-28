#include "commandInterpreter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static const char* stateToStr(int state);
static const char* typeToStr(int type);

static void printHelp(void) {
    printf("Available commands:\n");
    printf("  run <id>        - run program with numeric id (e.g. run 0)\n");
    printf("  run <file>      - run program file (e.g. run 0.txt)\n");
    printf("  ps              - list processes\n");
    printf("  help            - show this help\n");
    printf("  exit | quit     - terminate the OS\n");
}

void interpretCommand(Kernel* kernel, const char* command) {
    if (!command || strlen(command) == 0) {
        return;
    }

    /* ---------- EXIT / QUIT ---------- */
    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
        printf("Shutting down OS...\n");
        kernel->systemRunning = false;
        return;
    }

    /* ---------- HELP ---------- */
    if (strcmp(command, "help") == 0) {
        printHelp();
        return;
    }

    /* ---------- RUN ---------- */
    if (strncmp(command, "run ", 4) == 0) {
        const char* arg = command + 4;

        /* Case 1: numeric id (run 3) */
        if (isdigit((unsigned char)arg[0])) {
            int programId = (int)strtol(arg, NULL, 10);
            submitJob(kernel, programId);
            return;
        }

        /* Case 2: filename (run 3.txt) */
        char* endptr;
        int id = (int)strtol(arg, &endptr, 10);
        if (endptr && strcmp(endptr, ".txt") == 0) {
            submitJob(kernel, id);
            return;
        }

        printf("Invalid run argument: %s\n", arg);
        return;
    }

    if (strcmp(command, "ps") == 0) {
        printf("PID   TYPE     STATE         VM    QUEUE\n");
        printf("-------------------------------------------\n");

        /* Running process */
        if (kernel->runningProcess) {
            Process* p = kernel->runningProcess;
            printf("%-5d %-8s %-13s %-5s running\n",
                p->id,
                typeToStr(p->priority),
                stateToStr(p->state),
                p->vm ? "yes" : "-");
        }

        /* Ready system processes */
        for (int i = 0; i < kernel->readySystem->count; i++) {
            Process* p = kernel->readySystem->items[i];
            printf("%-5d %-8s %-13s %-5s readySystem\n",
                p->id,
                typeToStr(p->priority),
                stateToStr(p->state),
                p->vm ? "yes" : "-");
        }

        /* Ready user processes */
        for (int i = 0; i < kernel->readyUser->count; i++) {
            Process* p = kernel->readyUser->items[i];
            printf("%-5d %-8s %-13s %-5s readyUser\n",
                p->id,
                typeToStr(p->priority),
                stateToStr(p->state),
                p->vm ? "yes" : "-");
        }

        return;
    }

    /* ---------- UNKNOWN ---------- */
    printf("Unknown command: %s\n", command);
    printf("Type 'help' to see available commands.\n");
}

static const char* stateToStr(int state) {
    switch (state) {
        case RUNNING:        return "RUNNING";
        case READY:          return "READY";
        case BLOCKED:        return "BLOCKED";
        case READY_STOP:     return "READY_STOP";
        case BLOCKED_STOP:   return "BLOCKED_STOP";
        case TERIMINATED:    return "TERMINATED";
        default:             return "UNKNOWN";
    }
}

static const char* typeToStr(int type) {
    switch (type) {
        case T_SYSTEM: return "SYSTEM";
        case T_USER:   return "USER";
        default:       return "UNKNOWN";
    }
}