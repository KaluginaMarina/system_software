#include <stdio.h>
#include <getopt.h>
#include "cat.h"

int main(int args, char *argv[]) {
    unsigned int flags = parse_flags(args, argv);
    char *filename = NULL;
    while(optind != args) {
        filename = argv[optind];
        optind++;
        cat(flags, filename);
    }
    if (flags & FLAG_STDIN) {
        cat(flags, filename);
    }
    return EXIT_SUCCESS;
}

