#include <stdio.h>
#include "cat.h"

int main(int args, char *argv[]) {
    int flags = parse_flags(args, argv);
    printf("%d", flags);
    return 0;
}