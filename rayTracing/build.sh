#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -O1"
DFLAGS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

gcc $CFLAGS -o rayTracing main.c $DFLAGS