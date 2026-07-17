#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <gelf.h>

#include "elftool.h"

int elftool_check_if_has_dynamic_section(const char * fp) {
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "libelf initialization failed: %s\n", elf_errmsg(-1));
        return ELFTOOL_ERROR;
    }

    int fd = open(fp, O_RDONLY, 0);

    if (fd == -1) {
        perror(fp);
        return ELFTOOL_ERROR;
    }

    Elf * elf = elf_begin(fd, ELF_C_READ, NULL);

    if (elf == NULL) {
        fprintf(stderr, "elf_begin() failed: %s\n", elf_errmsg(-1));
        close(fd);
        return ELFTOOL_ERROR;
    }

    if (elf_kind(elf) != ELF_K_ELF) {
        fprintf(stderr, "NOT an ELF file: %s\n", fp);
        elf_end(elf);
        close(fd);
        return ELFTOOL_ERROR;
    }

    size_t phnum;

    if (elf_getphdrnum(elf, &phnum) != 0) {
        fprintf(stderr, "elf_getphdrnum() failed: %s\n", elf_errmsg(-1));
        elf_end(elf);
        close(fd);
        return ELFTOOL_ERROR;
    }

    GElf_Phdr phdr;

    for (size_t i = 0U; i < phnum; i++) {
        if (gelf_getphdr(elf, i, &phdr) != &phdr) {
            fprintf(stderr, "getphdr() failed: %s.", elf_errmsg(-1));
            elf_end(elf);
            close(fd);
            return ELFTOOL_ERROR;
        }

        if (phdr.p_type == PT_DYNAMIC) {
            elf_end(elf);
            close(fd);
            return ELFTOOL_OK;
        }
    }

    elf_end(elf);
    close(fd);

    fprintf(stderr, "no .dynamic section in file: %s\n", fp);
    return 200;
}
