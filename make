#!/usr/bin/env bash

CFLAGS="-Wall -Wextra"

# void
LDFLAGS="-lxbps"

# artix
# LDFLAGS="-lalpm -DARCH -DMULTILIB -DARTIX"

CONFIG=./config

distro=$1
if [ -z $distro ]; then distro="void"; fi

set -xe

cc -o flag_generator "$CONFIG/flag_generator.c" $CFLAGS
./flag_generator "$CONFIG/$distro.c"
cc -o dpacker "$CONFIG/$distro.c" $CFLAGS $LDFLAGS
