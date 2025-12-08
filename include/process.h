// process.h
#ifndef PROCESS_H
#define PROCESS_H

typedef enum {
    NEW,
    READY,
    RUNNING,
    FINISHED
} state_t;

typedef struct process {
    int pid;
    char name[32];
    state_t state;
    int burst;
    int remaining_time;
    int start_time;
    int end_time;
} process_t;

process_t* create_process(const char* name);
int create_process_sched(const char* name, int burst);
void list_processes();

#endif
