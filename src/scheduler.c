#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/process.h"

static Process* queue[32];
static int count = 0;

void scheduler_add(Process* p) {
    queue[count++] = p;
}

void* scheduler_loop(void* _arg) {
    int i = 0;
    while (1) {
        if (count == 0) continue;

        Process* p = queue[i];
        strcpy(p->state, "RUNNING");
        printf("[Scheduler] Running process: %s (PID=%d)\n", p->name, p->pid);
        sleep(1);
        strcpy(p->state, "READY");

        i = (i + 1) % count;
    }
}

void scheduler_start() {
    pthread_t t;
    pthread_create(&t, NULL, scheduler_loop, NULL);
}
