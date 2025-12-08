#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/shell.h"

#define MAX_PROCESSES 32

// ----------------------
// Scheduler variables
// ----------------------
static pthread_t scheduler_thread;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int running = 0;
static int scheduler_started = 0;
static int CURRENT_TIME = 0;
static int SCHED_ALG = ALG_RR;
static int QUANTUM = 1;

static process_t* queue[MAX_PROCESSES];
static int process_count = 0;

// ----------------------
// Helper: Convert state to string
// ----------------------
static const char* state_to_string(state_t state) {
    switch (state) {
        case NEW: return "NEW";
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case FINISHED: return "FINISHED";
        default: return "UNKNOWN";
    }
}

// ----------------------
// Helper: Pick processes
// ----------------------
static int last_scheduled_index = -1;

static process_t* pick_rr() {
    if (process_count == 0) return NULL;
    
    // Round-robin: start from the next process after the last scheduled one
    int start = (last_scheduled_index + 1) % process_count;
    
    for (int i = 0; i < process_count; i++) {
        int idx = (start + i) % process_count;
        if (queue[idx]->state == NEW || queue[idx]->state == READY) {
            last_scheduled_index = idx;
            return queue[idx];
        }
    }
    return NULL;
}

static process_t* pick_fcfs() {
    process_t* earliest = NULL;
    for (int i = 0; i < process_count; i++) {
        if ((queue[i]->state == NEW || queue[i]->state == READY) &&
            (!earliest || queue[i]->pid < earliest->pid))
            earliest = queue[i];
    }
    return earliest;
}

static process_t* pick_sjf() {
    process_t* shortest = NULL;
    for (int i = 0; i < process_count; i++) {
        if ((queue[i]->state == NEW || queue[i]->state == READY) &&
            (!shortest || queue[i]->remaining_time < shortest->remaining_time))
            shortest = queue[i];
    }
    return shortest;
}

// ----------------------
// Scheduler loop
// ----------------------
static void* scheduler_loop(void* _arg) {
    (void)_arg; // Mark as unused
    
    while (running) {
        pthread_mutex_lock(&lock);
        process_t *p = NULL;

        if (SCHED_ALG == ALG_SJF) p = pick_sjf();
        else if (SCHED_ALG == ALG_FCFS) p = pick_fcfs();
        else p = pick_rr();

        if (!p) {
            pthread_mutex_unlock(&lock);
            usleep(200000);
            continue;
        }

        // Only print when process FIRST starts (not on every resume)
        int is_first_start = (p->start_time == -1);
        if (is_first_start) {
            p->start_time = CURRENT_TIME;
            printf("\n[Scheduler] Process %s (PID=%d) started\n", p->name, p->pid);
            shell_redraw_prompt();
        }
        
        p->state = RUNNING;
        pthread_mutex_unlock(&lock);

        int slice = (SCHED_ALG == ALG_RR) ? QUANTUM : p->remaining_time;
        sleep(slice);
        CURRENT_TIME += slice;

        pthread_mutex_lock(&lock);
        p->remaining_time -= slice;

        if (p->remaining_time <= 0) {
            p->state = FINISHED;
            p->end_time = CURRENT_TIME;
            printf("\n[Scheduler] Process %s (PID=%d) finished (Start=%d End=%d)\n",
                   p->name, p->pid, p->start_time, p->end_time);
            shell_redraw_prompt();
        } else {
            p->state = READY; // back to ready
        }

        pthread_mutex_unlock(&lock);
        usleep(200000);
    }
    return NULL;
}

// ----------------------
// Scheduler API
// ----------------------
void scheduler_init(sched_algorithm_t alg, int quantum) {
    pthread_mutex_lock(&lock);
    SCHED_ALG = alg;
    QUANTUM = quantum;
    pthread_mutex_unlock(&lock);
}

void scheduler_add(process_t* p) {
    pthread_mutex_lock(&lock);
    if (process_count < MAX_PROCESSES)
        queue[process_count++] = p;
    pthread_mutex_unlock(&lock);
}

int create_process_sched(const char* name, int burst) {
    process_t* p = malloc(sizeof(process_t));
    p->pid = (process_count == 0) ? 1 : queue[process_count-1]->pid + 1;
    strncpy(p->name, name, 32);
    p->name[31] = '\0'; // Ensure null termination
    p->burst = burst;
    p->remaining_time = burst;
    p->state = NEW;
    p->start_time = -1;
    p->end_time = -1;

    scheduler_add(p);
    return p->pid;
}

void scheduler_start() {
    if (!scheduler_started) {
        scheduler_started = 1;
        running = 1;
        pthread_create(&scheduler_thread, NULL, scheduler_loop, NULL);
        printf("Scheduler started.\n");
    } else {
        printf("Scheduler is already running.\n");
    }
}

void scheduler_stop() {
    if (scheduler_started) {
        running = 0;
        pthread_join(scheduler_thread, NULL);
        scheduler_started = 0;
        printf("Scheduler stopped.\n");
    } else {
        printf("Scheduler is not running.\n");
    }
}

void scheduler_kill(int pid) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < process_count; i++) {
        if (queue[i]->pid == pid) {
            printf("[Scheduler] Process %s (PID=%d) killed\n", queue[i]->name, pid);
            free(queue[i]);
            for (int j = i; j < process_count - 1; j++)
                queue[j] = queue[j + 1];
            process_count--;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void scheduler_top() {
    pthread_mutex_lock(&lock);
    if (process_count == 0) {
        printf("No processes in scheduler queue.\n");
    } else {
        printf("PID\tName\t\tState\t\tBurst\tRemaining\n");
        printf("---\t----\t\t-----\t\t-----\t---------\n");
        for (int i = 0; i < process_count; i++) {
            process_t* p = queue[i];
            printf("%d\t%-12s\t%-12s\t%d\t%d\n", 
                   p->pid, p->name, state_to_string(p->state), 
                   p->burst, p->remaining_time);
        }
    }
    pthread_mutex_unlock(&lock);
}

void scheduler_ps() {
    pthread_mutex_lock(&lock);
    if (process_count == 0) {
        printf("No processes in scheduler queue.\n");
    } else {
        printf("PID\tName\t\tState\n");
        printf("---\t----\t\t-----\n");
        for (int i = 0; i < process_count; i++) {
            process_t* p = queue[i];
            printf("%d\t%-12s\t%s\n", p->pid, p->name, state_to_string(p->state));
        }
    }
    pthread_mutex_unlock(&lock);
}