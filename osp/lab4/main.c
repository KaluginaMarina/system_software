#include <stdio.h>
#include "cat.h"

int main(int args, char *argv[]) {
    unsigned int flags = parse_flags(args, argv);
    printf("-n %d -d %d -t %d -h %d\n", flags & FLAG_NUMBERS, flags & FLAG_HEAD, flags & FLAG_TAIL, flags & FLAG_HELP);
    return 0;
}