#include "../include/start.h"
#include "../include/shell.h"

void kernel_main(void) {
    start();

    shell_init();
    shell_run();
}