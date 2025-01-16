#!/bin/bash

# SPDX-License-Identifier: MIT
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
    local config=$1
    local build=$2
    local cmakeflags="-DCMAKE_BUILD_TYPE=$config"

    if [[ $build == "aie2" ]]; then
      cmakeflags="$cmakeflags -DAIEBU_FULL=OFF"
    else
      cmakeflags="$cmakeflags -DAIEBU_FULL=ON"
    fi

    mkdir -p $config
    cd $config
    if [[ $config == "Debug" ]]; then
	cmakeflags="$cmakeflags -DXRT_CLANG_TIDY=ON"
    fi

    cmake $cmakeflags ../../

    make -j $CORE VERBOSE=1 DESTDIR=$PWD
    if [[ $build != "aie2" ]]; then
      make -j $CORE VERBOSE=1 DESTDIR=$PWD isa-spec
    fi
    make -j $CORE VERBOSE=1 DESTDIR=$PWD install
    make -j $CORE VERBOSE=1 DESTDIR=$PWD test
    make -j $CORE VERBOSE=1 DESTDIR=$PWD test ARGS="-L memcheck -T memcheck"
    if [[ $config == "Release" ]]; then
	make -j $CORE VERBOSE=1 DESTDIR=$PWD package
    fi
}

build=""
usage() { echo "Usage: $0 [-r]" 1>&2; exit 1; }

while getopts ":rh" o; do
    case "${o}" in
        r)
            build="aie2"
            ;;
        h)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

cd $BUILDDIR
compile Debug $build

cd $BUILDDIR
compile Release $build

cd $here
