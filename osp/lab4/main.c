#include <stdio.h>
#include <getopt.h>
#include "cat.h"

int main(int args, char *argv[]) {
    unsigned int flags = parse_flags(args, argv);
    if (!(flags & FLAG_STDIN)) {
        char *filename = argv[optind];
        printf("%s\n", filename);
    }
    printf("-n %d -d %d -t %d -h %d -stdin %d\n", flags & FLAG_NUMBERS, flags & FLAG_HEAD, flags & FLAG_TAIL, flags & FLAG_HELP, flags & FLAG_STDIN);
    return 0;
}