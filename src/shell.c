#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/filesystem.h"
#include "../include/shell.h"

void shell_loop() {
    char cmd[128];

    while (1) {
        printf("mini-os> ");
        fgets(cmd, 128, stdin);

        if (strncmp(cmd, "create", 6) == 0) {
            char name[32];
            sscanf(cmd, "create %s", name);
            Process* p = create_process(name);
            scheduler_add(p);
        }
        else if (strcmp(cmd, "ps\n") == 0) {
            list_processes();
        }
        else if (strncmp(cmd, "touch", 5) == 0) {
            char name[32];
            sscanf(cmd, "touch %s", name);
            fs_touch(name);
        }
        else if (strncmp(cmd, "write", 5) == 0) {
            char f[32], text[64];
            sscanf(cmd, "write %s %s", f, text);
            fs_write(f, text);
        }
        else if (strncmp(cmd, "cat", 3) == 0) {
            char name[32];
            sscanf(cmd, "cat %s", name);
            fs_cat(name);
        }
        else if (strcmp(cmd, "ls\n") == 0) {
            fs_ls();
        }
        else if (strcmp(cmd, "exit\n") == 0) {
            break;
        }
        else {
            printf("Unknown command\n");
        }
    }
}
