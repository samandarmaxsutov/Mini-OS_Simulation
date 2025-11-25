#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    char name[32];
    char state[16];
} Process;

Process* create_process(const char* name);
void list_processes();

#endif
