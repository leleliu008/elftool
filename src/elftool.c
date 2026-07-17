#include <stdio.h>
#include <string.h>

#include "elftool.h"

#ifndef ELFTOOL_VERSION_STRING
#define ELFTOOL_VERSION_STRING "1.0.0"
#endif

typedef int (*fn)(const char * fp);

typedef struct {
    const char * arg;
    const fn func;
} Action;

int main(int argc, char* argv[]) {
    if (argc == 1) {
        elftool_print_help();
        return ELFTOOL_ERROR;
    }

    if (argv[1][0] == '\0') {
        elftool_print_help();
        return ELFTOOL_ERROR;
    }

    if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
        elftool_print_help();
        return ELFTOOL_OK;
    }

    if ((strcmp(argv[1], "-V") == 0) || (strcmp(argv[1], "--version") == 0)) {
        printf("%s\n", ELFTOOL_VERSION_STRING);
        return ELFTOOL_OK;
    }

    const Action actions[] = {
        {"print-interpreter", elftool_print_interpreter},
        {"print-soname", elftool_print_soname},
        {"print-needed", elftool_print_needed},
        {"print-rpath",  elftool_print_rpath},
        {NULL, NULL}
    };

    for (int i = 0; ; i++) {
        if (actions[i].arg == NULL) {
            fprintf(stderr, "%s: unrecognized action: %s\n", argv[0], argv[1]);
            elftool_print_help();
            return ELFTOOL_ERROR_ARG_IS_UNKNOWN;
        }

        if (strcmp(argv[1], actions[i].arg) == 0) {
            if (argv[2] == NULL) {
                fprintf(stderr, "Usage : %s %s <FILEPATH>, <FILEPATH> is unspecified.\n", argv[0], argv[1]);
                return ELFTOOL_ERROR_ARG_IS_NULL;
            }

            if (argv[2][0] == '\0') {
                fprintf(stderr, "Usage : %s %s <FILEPATH>, <FILEPATH> should be a non-empty string.\n", argv[0], argv[1]);
                return ELFTOOL_ERROR_ARG_IS_EMPTY;
            }

            return actions[i].func(argv[2]);
        }
    }
}
