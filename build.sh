#!/bin/sh

set -ex

exec ${CC:-cc} $CPPFLAGS ${CFLAGS:--std=gnu99 -Os} ${LDFLAGS:--flto -Wl,-s} src/*.c -lelf "$@"
