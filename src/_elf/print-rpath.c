#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include <elf.h>

#include "../elftool.h"

static int handle_elf32(unsigned char * elf) {
    Elf32_Ehdr * ehdr = (Elf32_Ehdr*)elf;
    Elf32_Phdr * phdr = NULL;

    int hasDynamic = 0;

    for (Elf32_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf32_Phdr *)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type == PT_DYNAMIC) {
            hasDynamic = 1;
            break;
        }
    }

    if (hasDynamic == 0) return 0;

    //////////////////////////////////////////

    Elf32_Addr addr = 0;
    Elf32_Dyn * dyn;

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf32_Dyn*)(elf + phdr->p_offset + i * sizeof(Elf32_Dyn));

        if (dyn->d_tag == DT_NULL) {
            break;
        }

        if (dyn->d_tag == DT_STRTAB) {
            addr = dyn->d_un.d_ptr;
            break;
        }
    }

    if (addr == 0) {
        return 1;
    }

    //////////////////////////////////////////

    const char * dynstr = NULL;

    Elf32_Phdr * phdr2;

    for (Elf32_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr2 = (Elf32_Phdr*)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr2->p_type == PT_LOAD) {
            Elf32_Addr a = phdr2->p_vaddr;
            Elf32_Addr b = phdr2->p_memsz + a;

            if (addr >= a && addr < b) {
                dynstr = (const char *)elf + phdr2->p_offset + (addr - a);
                break;
            }
        }
    }

    if (dynstr == NULL) {
        return 1;
    }

    //////////////////////////////////////////

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf32_Dyn*)(elf + phdr->p_offset + i * sizeof(Elf32_Dyn));

        if (dyn->d_tag == DT_NULL) {
            break;
        }

        if (dyn->d_tag == DT_RUNPATH) {
            puts(dynstr + dyn->d_un.d_val);
        }
    }

    return 0;
}

static int handle_elf64(unsigned char * elf) {
    Elf64_Ehdr * ehdr = (Elf64_Ehdr*)elf;
    Elf64_Phdr * phdr = NULL;

    int hasDynamic = 0;

    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf64_Phdr *)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type == PT_DYNAMIC) {
            hasDynamic = 1;
            break;
        }
    }

    if (hasDynamic == 0) return 0;

    //////////////////////////////////////////

    Elf64_Addr addr = 0;
    Elf64_Dyn * dyn;

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf64_Dyn*)(elf + phdr->p_offset + i * sizeof(Elf64_Dyn));

        if (dyn->d_tag == DT_NULL) {
            break;
        }

        if (dyn->d_tag == DT_STRTAB) {
            addr = dyn->d_un.d_ptr;
            break;
        }
    }

    if (addr == 0) {
        return 1;
    }

    //////////////////////////////////////////

    const char * dynstr = NULL;

    Elf64_Phdr * phdr2;

    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
        phdr2 = (Elf64_Phdr*)(elf + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr2->p_type == PT_LOAD) {
            Elf64_Addr a = phdr2->p_vaddr;
            Elf64_Addr b = phdr2->p_memsz + a;

            if (addr >= a && addr < b) {
                dynstr = (const char *)elf + phdr2->p_offset + (addr - a);
                break;
            }
        }
    }

    if (dynstr == NULL) {
        return 1;
    }

    //////////////////////////////////////////

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf64_Dyn*)(elf + phdr->p_offset + i * sizeof(Elf64_Dyn));

        if (dyn->d_tag == DT_NULL) {
            break;
        }

        if (dyn->d_tag == DT_RUNPATH) {
            puts(dynstr + dyn->d_un.d_val);
        }
    }

    return 0;
}

int elftool_print_rpath(const char * fp) {
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

    int ret;

    switch (a[4]) {
        case ELFCLASS32: ret = handle_elf32(elf); break;
        case ELFCLASS64: ret = handle_elf64(elf); break;
        default: 
            fprintf(stderr, "Invalid ELF file: %s\n", fp);
            ret = 101;
    }

    return ret;
}
