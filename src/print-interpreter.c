#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include <elf.h>

#include "elftool.h"

static int handle_elf32(unsigned char * const elf) {
    Elf32_Ehdr * ehdr = (Elf32_Ehdr *)elf;

    Elf32_Phdr * phdr;

    for (Elf32_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf32_Phdr *)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type == PT_INTERP) {
            puts((char*)(elf + phdr->p_offset));
            break;
        }
    }

    return 0;
}

static int handle_elf32_swap(unsigned char * const elf) {
    Elf32_Ehdr * ehdr = (Elf32_Ehdr *)elf;

    Elf32_Phdr * phdr;

    uint16_t phnum = __builtin_bswap16(ehdr->e_phnum);
    uint32_t phoff = __builtin_bswap32(ehdr->e_phoff);
    uint16_t phentsize = __builtin_bswap16(ehdr->e_phentsize);

    for (uint16_t i = 0; i < phnum; i++) {
        phdr = (Elf32_Phdr *)(elf + phoff + i * phentsize);

        if (__builtin_bswap32(phdr->p_type) == PT_INTERP) {
            puts((char*)(elf + __builtin_bswap32(phdr->p_offset)));
            break;
        }
    }

    return 0;
}

static int handle_elf64(unsigned char * const elf) {
    Elf64_Ehdr * ehdr = (Elf64_Ehdr *)elf;

    Elf64_Phdr * phdr;

    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf64_Phdr *)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type == PT_INTERP) {
            puts((char*)(elf + phdr->p_offset));
            break;
        }
    }

    return 0;
}

static int handle_elf64_swap(unsigned char * const elf) {
    Elf64_Ehdr * ehdr = (Elf64_Ehdr *)elf;

    Elf64_Phdr * phdr;

    uint16_t phnum = __builtin_bswap16(ehdr->e_phnum);
    uint64_t phoff = __builtin_bswap64(ehdr->e_phoff);
    uint16_t phentsize = __builtin_bswap16(ehdr->e_phentsize);

    for (uint16_t i = 0; i < phnum; i++) {
        phdr = (Elf64_Phdr *)(elf + phoff + i * phentsize);

        if (__builtin_bswap32(phdr->p_type) == PT_INTERP) {
            puts((char*)(elf + __builtin_bswap64(phdr->p_offset)));
            break;
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

    unsigned char a[6];

    ssize_t readBytes = read(fd, a, 6);

    if (readBytes == -1) {
        perror(fp);
        close(fd);
        return 5;
    }

    if (readBytes != 6) {
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

    int swap = 0;

    switch (a[5]) {
        case ELFDATA2LSB:
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            swap = 1;
#endif
            break;
        case ELFDATA2MSB:
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            swap = 1;
#endif
            break;
        default:
            fprintf(stderr, "Invalid ELF file: %s\n", fp);
            return 101;
    }

    ///////////////////////////////////////////////////////////

    void * p = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
     
    if (p == MAP_FAILED) {
        perror(fp);
        close(fd);
        return 200;
    }

    ///////////////////////////////////////////////////////////

    close(fd);

    int ret;

    switch (a[4]) {
        case ELFCLASS32:
            if (swap == 0) {
                ret = handle_elf32(p);
            } else {
                ret = handle_elf32_swap(p);
            }
            break;
        case ELFCLASS64:
            if (swap == 0) {
                ret = handle_elf64(p);
            } else {
                ret = handle_elf64_swap(p);
            }
            break;
        default: 
            fprintf(stderr, "Invalid ELF file: %s\n", fp);
            ret = 101;
    }

    munmap(p, st.st_size);

    return ret;
}
