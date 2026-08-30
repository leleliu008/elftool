#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include <elf.h>

#include <elftool.h>

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

        if (dyn->d_tag == DT_SONAME) {
            puts(dynstr + dyn->d_un.d_val);
            break;
        }
    }

    return 0;
}

static int handle_elf32_swap(unsigned char * elf) {
    Elf32_Ehdr * ehdr = (Elf32_Ehdr*)elf;
    Elf32_Phdr * phdr = NULL;

    int hasDynamic = 0;

    uint16_t phnum = __builtin_bswap16(ehdr->e_phnum);
    uint32_t phoff = __builtin_bswap32(ehdr->e_phoff);
    uint16_t phentsize = __builtin_bswap16(ehdr->e_phentsize);

    for (uint16_t i = 0; i < phnum; i++) {
        phdr = (Elf32_Phdr *)(elf + phoff + i * phentsize);

        if (__builtin_bswap32(phdr->p_type) == PT_DYNAMIC) {
            hasDynamic = 1;
            break;
        }
    }

    if (hasDynamic == 0) return 0;

    //////////////////////////////////////////

    Elf32_Addr addr = 0;
    Elf32_Dyn * dyn;

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf32_Dyn*)(elf + __builtin_bswap32(phdr->p_offset) + i * sizeof(Elf32_Dyn));

        int32_t tag = __builtin_bswap32(dyn->d_tag);

        if (tag == DT_NULL) {
            break;
        }

        if (tag == DT_STRTAB) {
            addr = __builtin_bswap32(dyn->d_un.d_ptr);
            break;
        }
    }

    if (addr == 0) {
        return 1;
    }

    //////////////////////////////////////////

    const char * dynstr = NULL;

    Elf32_Phdr * phdr2;

    for (uint16_t i = 0; i < phnum; i++) {
        phdr2 = (Elf32_Phdr*)(elf + phoff + i * phentsize);

        if (__builtin_bswap32(phdr2->p_type) == PT_LOAD) {
            uint32_t a = __builtin_bswap32(phdr2->p_vaddr);
            uint32_t b = __builtin_bswap32(phdr2->p_memsz) + a;

            if (addr >= a && addr < b) {
                dynstr = (const char *)elf + __builtin_bswap32(phdr2->p_offset) + (addr - a);
                break;
            }
        }
    }

    if (dynstr == NULL) {
        return 1;
    }

    //////////////////////////////////////////

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf32_Dyn*)(elf + __builtin_bswap32(phdr->p_offset) + i * sizeof(Elf32_Dyn));

        int32_t tag = __builtin_bswap32(dyn->d_tag);

        if (tag == DT_NULL) {
            break;
        }

        if (tag == DT_SONAME) {
            puts(dynstr + __builtin_bswap32(dyn->d_un.d_val));
            break;
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

        if (dyn->d_tag == DT_SONAME) {
            puts(dynstr + dyn->d_un.d_val);
            break;
        }
    }

    return 0;
}

static int handle_elf64_swap(unsigned char * elf) {
    Elf64_Ehdr * ehdr = (Elf64_Ehdr*)elf;
    Elf64_Phdr * phdr = NULL;

    int hasDynamic = 0;

    uint16_t phnum = __builtin_bswap16(ehdr->e_phnum);
    uint64_t phoff = __builtin_bswap64(ehdr->e_phoff);
    uint16_t phentsize = __builtin_bswap16(ehdr->e_phentsize);

    for (uint16_t i = 0; i < phnum; i++) {
        phdr = (Elf64_Phdr *)(elf + phoff + i * phentsize);

        if (__builtin_bswap32(phdr->p_type) == PT_DYNAMIC) {
            hasDynamic = 1;
            break;
        }
    }

    if (hasDynamic == 0) return 0;

    //////////////////////////////////////////

    Elf64_Addr addr = 0;
    Elf64_Dyn * dyn;

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf64_Dyn*)(elf + __builtin_bswap64(phdr->p_offset) + i * sizeof(Elf64_Dyn));

        int64_t tag = __builtin_bswap64(dyn->d_tag);

        if (tag == DT_NULL) {
            break;
        }

        if (tag == DT_STRTAB) {
            addr = __builtin_bswap64(dyn->d_un.d_ptr);
            break;
        }
    }

    if (addr == 0) {
        return 1;
    }

    //////////////////////////////////////////

    const char * dynstr = NULL;

    Elf64_Phdr * phdr2;

    for (uint16_t i = 0; i < phnum; i++) {
        phdr2 = (Elf64_Phdr*)(elf + phoff + i * phentsize);

        if (__builtin_bswap32(phdr2->p_type) == PT_LOAD) {
            Elf64_Addr a = __builtin_bswap64(phdr2->p_vaddr);
            Elf64_Addr b = __builtin_bswap64(phdr2->p_memsz) + a;

            if (addr >= a && addr < b) {
                dynstr = (const char *)elf + __builtin_bswap64(phdr2->p_offset) + (addr - a);
                break;
            }
        }
    }

    if (dynstr == NULL) {
        return 1;
    }

    //////////////////////////////////////////

    for (size_t i = 0; i < 100; i++) {
        dyn = (Elf64_Dyn*)(elf + __builtin_bswap64(phdr->p_offset) + i * sizeof(Elf64_Dyn));

        int64_t tag = __builtin_bswap64(dyn->d_tag);

        if (tag == DT_NULL) {
            break;
        }

        if (tag == DT_SONAME) {
            puts(dynstr + __builtin_bswap64(dyn->d_un.d_val));
            break;
        }
    }

    return 0;
}

int elftool_print_soname(const char * fp) {
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
