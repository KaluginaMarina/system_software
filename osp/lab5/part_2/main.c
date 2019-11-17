#include <stdio.h>
#include "threads.h"

int main(int argc, char *argv[]) {
    start(argc, argv);
    change_reg();
    reverse();
    print_array();
    return 0;
}