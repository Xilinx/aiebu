#!/bin/bash

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set -e

OSDIST=`grep '^ID=' /etc/os-release | awk -F= '{print $2}' | tr -d '"'`
VERSION=`grep '^VERSION_ID=' /etc/os-release | awk -F= '{print $2}' | tr -d '"'`
MAJOR=${VERSION%.*}
BUILDDIR=$(readlink -f $(dirname ${BASH_SOURCE[0]}))
CORE=`grep -c ^processor /proc/cpuinfo`
CMAKE=cmake
CMAKE_MAJOR_VERSION=`cmake --version | head -n 1 | awk '{print $3}' |awk -F. '{print $1}'`
CPU=`uname -m`

here=$PWD

function compile {
    config=$1

    mkdir -p $config
    cd $config
    cmake -DCMAKE_BUILD_TYPE=$config ../../

    make VERBOSE=1 DESTDIR=$PWD
    make VERBOSE=1 DESTDIR=$PWD test
    make VERBOSE=1 DESTDIR=$PWD isa-spec
    make VERBOSE=1 DESTDIR=$PWD install
    if [[ $config == "Release" ]]; then
	make VERBOSE=1 DESTDIR=$PWD package
    fi
}

cd $BUILDDIR
compile Debug

cd $BUILDDIR
compile Release

cd $here
