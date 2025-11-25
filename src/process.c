#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

static int NEXT_PID = 1;
static Process* processes[32];
static int process_count = 0;

Process* create_process(const char* name) {
    Process* p = malloc(sizeof(Process));
    p->pid = NEXT_PID++;
    strncpy(p->name, name, 32);
    strcpy(p->state, "READY");

    processes[process_count++] = p;
    return p;
}

void list_processes() {
    printf("PID\tName\tState\n");
    for (int i = 0; i < process_count; i++) {
        Process* p = processes[i];
        printf("%d\t%s\t%s\n", p->pid, p->name, p->state);
    }
}
