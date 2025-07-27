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
run_memtest="no"

function compile {
    local config=$1
    local build_python=$2
    local cmakeflags="-DCMAKE_BUILD_TYPE=$config"
    if [[ $build_python == "yes" ]]; then
      cmakeflags="$cmakeflags -DAIEBU_PYTHON=ON"
    fi

    if [[ $config == "Debug" ]]; then
	cmakeflags="$cmakeflags -DXRT_CLANG_TIDY=ON"
    fi

    cmake -B $BUILDDIR/$config $cmakeflags $BUILDDIR/..
    cmake --build $BUILDDIR/$config --config $config --verbose -j $CORE
    cmake --install $BUILDDIR/$config --config $config --prefix $BUILDDIR/$config/opt/xilinx/aiebu
    cmake --build $BUILDDIR/$config --config $config --target test

    if [[ $run_memtest == "yes" ]]; then
        cmake --build $BUILDDIR/$config --target test -- ARGS="-L memcheck -T memcheck"
    fi
    if [[ $config == "Release" ]]; then
        cmake --build $BUILDDIR/$config --config $config --target package --verbose -j $CORE
    fi
}

build_python="yes"
usage() { echo "Usage: $0 [-ph]" 1>&2; exit 1; }

while getopts ":rph" o; do
    case "${o}" in
        p)
            build_python="yes"
            ;;
	m)
	    run_memtest="yes"
	    ;;
        h)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

compile Debug $build_python

compile Release $build_python
