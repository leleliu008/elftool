#!/bin/sh

set -ex

unset IFS

exec ${CC:-cc} $CPPFLAGS ${CFLAGS:--std=gnu99 -Os} ${LDFLAGS:--flto -Wl,-s} -o elftool src/*.c -lelf "$@"
