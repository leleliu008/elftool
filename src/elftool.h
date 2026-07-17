#ifndef ELFTOOL_H
#define ELFTOOL_H

#define ELFTOOL_OK                     0
#define ELFTOOL_ERROR                  1

#define ELFTOOL_ERROR_ARG_IS_NULL      2
#define ELFTOOL_ERROR_ARG_IS_EMPTY     3
#define ELFTOOL_ERROR_ARG_IS_INVALID   4
#define ELFTOOL_ERROR_ARG_IS_UNKNOWN   5

#define ELFTOOL_ERROR_NOT_ELF_FILE    10

int elftool_print_help();

int elftool_print_interpreter(const char * fp);

int elftool_print_soname(const char * fp);

int elftool_print_needed(const char * fp);

int elftool_print_rpath(const char * fp);

#endif
