# elftool

A command-line tool to manipulate ELF files based on [libelf](https://github.com/Distrotech/libelf)

## Build elftool directly using C Compiler

```bash
cc src/*.c -o elftool -lelf -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib
```

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

## Build elftool via [HomeBrew](https://brew.sh/) on macOS

```bash
brew install libelf
```

## install dependencies via your system's package manager

**[Ubuntu](https://ubuntu.com/) | [Debian GNU/Linux](https://www.debian.org/) | [LinuxMint](https://linuxmint.com/)**

```bash
apt -y update
apt -y install gcc libelf-dev
```

**[Fedora](https://getfedora.org/) | [CentOS](https://www.centos.org/) | [Rocky Linux](https://rockylinux.org/)**

```bash
dnf -y update
dnf -y install gcc elfutils-libelf-devel
```

**[ArchLinux](https://archlinux.org/)**

```bash
pacman -Syyuu --noconfirm
pacman -S     --noconfirm gcc libelf
```

**[AlpineLinux](https://www.alpinelinux.org/)**

```bash
apk update
apk add gcc libc-dev elfutils-dev
```

**[VoidLinux](https://voidlinux.org/)**

```bash
xbps-install -Suy xbps
xbps-install -Suy gcc elfutils-devel
```

**[Gentoo Linux](https://www.gentoo.org/)**

```bash
emerge gcc dev-libs/libelf
```

**[openSUSE](https://www.opensuse.org/)**

```bash
zypper update  -y  
zypper install -y gcc libelf-devel
```

**[OmniOS](https://omnios.org/)**

```bash
pkg refresh
pkg install gcc14
```

**[MidnightBSD](https://www.midnightbsd.org/)**

```bash
mport index
mport install libelf
```

**[FreeBSD](https://www.freebsd.org/) | [DragonFly BSD](https://www.dragonflybsd.org/)**

```bash
pkg update
pkg install -y libelf
```

**[OpenBSD](https://www.openbsd.org/)**

```bash
pkg_add libelf
```

**[NetBSD](https://www.netbsd.org/)**

```bash
pkgin -y install libelf
```

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
    elftool print-soname /usr/bin/ls
    ```

- **print all needed dynamic libraries of the given ELF file**

    ```bash
    elftool print-needed /usr/bin/ls
    ```

- **print all RUNPATH encoded in the given ELF file**

    ```bash
    elftool print-rpath /usr/bin/ls
    ```
