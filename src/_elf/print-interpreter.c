#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include <elf.h>

#include "../elftool.h"

int handle_elf32(unsigned char * const elf) {
    Elf32_Ehdr * ehdr = (Elf32_Ehdr *)elf;

    Elf32_Phdr * phdr;

    for (Elf32_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf32_Phdr *)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type == PT_INTERP) {
            puts((char*)(elf + phdr->p_offset));
        }
    }

    return 0;
}

int handle_elf64(unsigned char * const elf) {
    Elf64_Ehdr * ehdr = (Elf64_Ehdr *)elf;

    Elf64_Phdr * phdr;

    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf64_Phdr *)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type == PT_INTERP) {
            puts((char*)(elf + phdr->p_offset));
        }
    }

    return 0;
}

int elftool_print_interpreter(const char * fp) {
    int fd = open(fp, O_RDONLY);

    if (fd == -1) {
        perror(fp);
        return 3;
    }

    struct stat st;

    if (fstat(fd, &st) == -1) {
        perror(fp);
        close(fd);
        return 4;
    }

    if (st.st_size < 52) {
        fprintf(stderr, "NOT an ELF file: %s\n", fp);
        close(fd);
        return 100;
    }

    ///////////////////////////////////////////////////////////

    unsigned char a[5];

    ssize_t readBytes = read(fd, a, 5);

    if (readBytes == -1) {
        perror(fp);
        close(fd);
        return 5;
    }

    if (readBytes != 5) {
        perror(fp);
        close(fd);
        fprintf(stderr, "not fully read.\n");
        return 6;
    }

    ///////////////////////////////////////////////////////////

    // https://www.sco.com/developers/gabi/latest/ch4.eheader.html
    if ((a[0] != 0x7F) || (a[1] != 0x45) || (a[2] != 0x4C) || (a[3] != 0x46)) {
        fprintf(stderr, "NOT an ELF file: %s\n", fp);
        close(fd);
        return 100;
    }

    ///////////////////////////////////////////////////////////

    unsigned char * elf = (unsigned char *)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
     
    if (elf == NULL) {
        close(fd);
        return 200;
    }

    ///////////////////////////////////////////////////////////

    close(fd);

    switch (a[4]) {
        case ELFCLASS32: return handle_elf32(elf);
        case ELFCLASS64: return handle_elf64(elf);
        default: 
            fprintf(stderr, "Invalid ELF file: %s\n", fp);
            return 101;
    }
}
