#include <stdio.h>
#include <getopt.h>
#include "cat.h"

int main(int args, char *argv[]) {
    unsigned int flags = parse_flags(args, argv);
    char *filename = NULL;
    if (!(flags & FLAG_STDIN)) {
        filename = argv[optind];
        printf("%s\n", filename);
    }
    cat(flags, filename);
    return EXIT_SUCCESS;
}