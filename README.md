# elftool

A command-line tool to manipulate ELF files.

## Build elftool via [ppkg](https://github.com/leleliu008/ppkg)

```bash
ppkg install elftool
```

## Build elftool via [xcpkg](https://github.com/leleliu008/xcpkg)

```bash
xcpkg install elftool
```

## Build elftool via [ndk-pkg](https://github.com/leleliu008/ndk-pkg)

```bash
ndk-pkg install elftool
```

## Build elftool via [./build.sh](https://github.com/leleliu008/elftool/blob/master/build.sh)

`./build.sh` accepts any compiler options. If no compiler options are given, `-std=gnu99 -Os -flto -Wl,-s -o elftool` will be passed to the C Compiler.

`./build.sh` will install [gcc](https://gcc.gnu.org/) and [libelf](https://github.com/Distrotech/libelf) via your system's package manager.

`./build.sh` has handled these operating systems: [Debian GNU/Linux](https://www.debian.org/), [Ubuntu](https://ubuntu.com/), [LinuxMint](https://linuxmint.com/), [Fedora](https://getfedora.org/), [CentOS](https://www.centos.org/), [Rocky Linux](https://rockylinux.org/), [AlmaLinux](https://almalinux.org/), [ArchLinux](https://archlinux.org/), [Manjaro Linux](https://manjaro.org/), [AlpineLinux](https://www.alpinelinux.org/), [VoidLinux](https://voidlinux.org/), [Gentoo Linux](https://www.gentoo.org/), [openSUSE](https://www.opensuse.org/), [MidnightBSD](https://www.midnightbsd.org/), [DragonFly BSD](https://www.dragonflybsd.org/), [FreeBSD](https://www.freebsd.org/), [OpenBSD](https://www.openbsd.org/), [NetBSD](https://www.netbsd.org/), [OmniOS](https://omnios.org/), macOS + [HomeBrew](https://brew.sh/)

## elftool command usage

- **show help of this command**

    ```bash
    elftool -h
    elftool --help
    ```

- **show version of this command**

    ```bash
    elftool -V
    elftool --version
    ```

- **print the interpreter of the given ELF file**

    ```bash
    elftool print-interpreter /usr/bin/ls
    ```

- **print the soname of the given ELF file**

    ```bash
    elftool print-soname /usr/lib64/ld-linux-x86-64.so.2
    ```

- **print all needed dynamic libraries of the given ELF file**

    ```bash
    elftool print-needed /usr/bin/ls
    ```

- **print all RUNPATH encoded in the given ELF file**

    ```bash
    elftool print-rpath /usr/bin/ls
    ```
