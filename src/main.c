#include "../include/shell.h"
#include "../include/scheduler.h"

int main() {
    scheduler_start();
    shell_loop();
    return 0;
}
