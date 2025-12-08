#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>
#include "process.h"

typedef enum {
    ALG_RR,
    ALG_SJF,
    ALG_FCFS
} sched_algorithm_t;

void scheduler_init(sched_algorithm_t alg, int quantum);
void scheduler_start();
void scheduler_stop();
void scheduler_top();
void scheduler_add(process_t* p);
void scheduler_kill(int pid);

#endif
