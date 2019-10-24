#include <getopt.h>
#include "cat.h"

int parse_flags(int argc, char *argv[]){
    int flags = 0, opt = 0;
    while ((opt = getopt(argc, argv, "nht")) != -1) {
        switch (opt) {
            case 'n':
                flags |= FLAG_NUMBERS;
                break;
            case 't':
                flags |= FLAG_TAIL;
                break;
            case 'h':
                flags |= FLAG_HEAD;
                break;
        }
    }
    return flags;
}