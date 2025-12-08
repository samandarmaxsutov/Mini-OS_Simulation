#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

// This file can be simplified since scheduler.c handles everything now
// Keep only if you need create_process for other purposes

static process_t* processes[32];
static int NEXT_PID = 1;

process_t* create_process(const char* name) {
    process_t* p = malloc(sizeof(process_t));
    p->pid = NEXT_PID++;
    strncpy(p->name, name, 32);
    p->name[31] = '\0';
    p->state = NEW;
    p->burst = 0;
    p->remaining_time = 0;
    p->start_time = -1;
    p->end_time = -1;
    
    for (int i = 0; i < 32; i++) {
        if (!processes[i]) {
            processes[i] = p;
            break;
        }
    }
    return p;
}

void list_processes() {
    printf("PID\tName\t\tState\n");
    printf("---\t----\t\t-----\n");
    for (int i = 0; i < 32; i++) {
        process_t* p = processes[i];
        if (p) {
            const char* s;
            switch (p->state) {
                case NEW: s = "NEW"; break;
                case READY: s = "READY"; break;
                case RUNNING: s = "RUNNING"; break;
                case FINISHED: s = "FINISHED"; break;
                default: s = "UNKNOWN"; break;
            }
            printf("%d\t%-12s\t%s\n", p->pid, p->name, s);
        }
    }
}