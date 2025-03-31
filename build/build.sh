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
    local cmakeflags="-DCMAKE_BUILD_TYPE=$config -DCMAKE_INSTALL_PREFIX=$PWD/$config"
    if [[ $build_python == "yes" ]]; then
      cmakeflags="$cmakeflags -DAIEBU_PYTHON=ON"
    fi

    mkdir -p $config
    cd $config
    if [[ $config == "Debug" ]]; then
	cmakeflags="$cmakeflags -DXRT_CLANG_TIDY=ON"
    fi

    cmake $cmakeflags ../../

    make -j $CORE VERBOSE=1
    make -j $CORE VERBOSE=1 install
    make -j $CORE VERBOSE=1 test
    if [[ $run_memtest == "yes" ]]; then
	make -j $CORE VERBOSE=1 test ARGS="-L memcheck -T memcheck"
    fi
    if [[ $config == "Release" ]]; then
	make -j $CORE VERBOSE=1 package
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

cd $BUILDDIR
compile Debug $build_python

cd $BUILDDIR
compile Release $build_python

cd $here
