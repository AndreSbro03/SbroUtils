#!/bin/sh
set -xe

CFLAGS="-Wall -Wextra"
DFLAGS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

gcc $CFLAGS -o Snake main.c $DFLAGS && ./Snake