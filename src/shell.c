#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/filesystem.h"
#include "../include/shell.h"

// Helper: trim newline
static void trim_newline(char *s) {
    s[strcspn(s, "\n")] = 0;
}

void shell_redraw_prompt() {
    printf("mini-os> ");
    fflush(stdout);
}

void shell_loop() {
    char cmd[256];

    printf("Welcome to MiniOS shell. Type 'help' for commands.\n");

    while (1) {
        shell_redraw_prompt();
        if (!fgets(cmd, sizeof(cmd), stdin))
            break;
        trim_newline(cmd);

        if (strlen(cmd) == 0) continue;

        // ----------------------
        // help
        // ----------------------
        if (strcmp(cmd, "help") == 0) {
            printf("Available commands:\n");
            printf("  touch <file>      - create file\n");
            printf("  ls                - list files\n");
            printf("  ls -l             - list files with metadata\n");
            printf("  cat <file>        - show file contents\n");
            printf("  write <file> <text> - write text to file\n");
            printf("  rm <file>         - delete file\n");
            printf("  create <name> <burst> - create process with burst time\n");
            printf("  start             - start scheduler\n");
            printf("  stop              - stop scheduler\n");
            printf("  top               - show process table\n");
            printf("  sched <rr|sjf|fcfs> [quantum] - set scheduling algorithm\n");
            printf("  kill <pid>        - kill process\n");
            printf("  ps                - show processes\n");
            printf("  exit              - quit shell\n");
        }

        // ----------------------
        // touch FILE
        // ----------------------
        else if (strncmp(cmd, "touch ", 6) == 0) {
            char name[32];
            sscanf(cmd + 6, "%s", name);
            fs_touch(name);
        }

        // ----------------------
        // ls
        // ----------------------
        else if (strcmp(cmd, "ls") == 0) {
            fs_ls();
        }
        else if (strcmp(cmd, "ls -l") == 0) {
            fs_ls_long();
        }

        // ----------------------
        // cat FILE
        // ----------------------
        else if (strncmp(cmd, "cat ", 4) == 0) {
            char name[32];
            sscanf(cmd + 4, "%s", name);
            fs_cat(name);
        }

        // ----------------------
        // rm FILE
        // ----------------------
        else if (strncmp(cmd, "rm ", 3) == 0) {
            char name[32];
            sscanf(cmd + 3, "%s", name);
            fs_delete(name);
        }

        // ----------------------
        // write FILE TEXT
        // ----------------------
        else if (strncmp(cmd, "write ", 6) == 0) {
            char file[32];
            char *space = strchr(cmd + 6, ' ');
            if (!space) {
                printf("Usage: write <file> <text>\n");
                continue;
            }
            *space = '\0';
            strcpy(file, cmd + 6);
            char *text = space + 1;
            fs_write(file, text);
        }

        // ----------------------
        // create process
        // ----------------------
        else if (strncmp(cmd, "create ", 7) == 0) {
            char name[32];
            int burst;
            if (sscanf(cmd + 7, "%s %d", name, &burst) != 2) {
                printf("Usage: create <name> <burst>\n");
                continue;
            }
            int pid = create_process_sched(name, burst);
            printf("Process '%s' created with PID=%d, burst=%d\n", name, pid, burst);
        }

        // ----------------------
        // start scheduler
        // ----------------------
        else if (strcmp(cmd, "start") == 0) {
            scheduler_start();
        }

        // ----------------------
        // stop scheduler
        // ----------------------
        else if (strcmp(cmd, "stop") == 0) {
            scheduler_stop();
        }

        // ----------------------
        // top
        // ----------------------
        else if (strcmp(cmd, "top") == 0) {
            scheduler_top();
        }

        // ----------------------
        // sched <rr|sjf|fcfs> [quantum]
        // ----------------------
        else if (strncmp(cmd, "sched ", 6) == 0) {
            char type[8];
            int quantum = 1;
            if (sscanf(cmd + 6, "%s %d", type, &quantum) < 1) {
                printf("Usage: sched <rr|sjf|fcfs> [quantum]\n");
                continue;
            }
            if (strcmp(type, "rr") == 0) scheduler_init(ALG_RR, quantum);
            else if (strcmp(type, "sjf") == 0) scheduler_init(ALG_SJF, 1);
            else if (strcmp(type, "fcfs") == 0) scheduler_init(ALG_FCFS, 1);
            else printf("Unknown algorithm: %s\n", type);
        }

        // ----------------------
        // ps
        // ----------------------
        else if (strcmp(cmd, "ps") == 0) {
            list_processes();
        }

        // ----------------------
        // kill
        // ----------------------
        else if (strncmp(cmd, "kill ", 5) == 0) {
            int pid = atoi(cmd + 5);
            scheduler_kill(pid);
        }

        // ----------------------
        // exit
        // ----------------------
        else if (strcmp(cmd, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // ----------------------
        // unknown command
        // ----------------------
        else {
            printf("Unknown command: %s\n", cmd);
        }
    }
}
