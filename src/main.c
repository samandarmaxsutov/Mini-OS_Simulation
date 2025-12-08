#include <stdio.h>
#include "../include/scheduler.h"
#include "../include/filesystem.h"
#include "../include/shell.h"

int main() {
    // Initialize scheduler with Round Robin algorithm and quantum of 1
    scheduler_init(ALG_RR, 1);
    // Start the shell
    shell_loop();
    return 0;
}