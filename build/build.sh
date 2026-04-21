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
run_test="yes"

function compile {
    local config=$1
    local build_python=$2
    local ctrlcode_codegen=$3
    local cmakeflags="-DCMAKE_BUILD_TYPE=$config"
    if [[ $build_python == "yes" ]]; then
      cmakeflags="$cmakeflags -DAIEBU_PYTHON=ON"
    fi

    if [[ $ctrlcode_codegen == "yes" ]]; then
      cmakeflags="$cmakeflags -DAIEBU_CTRLCODE_CODEGEN=ON"
    fi

    if [[ $config == "Debug" ]]; then
	cmakeflags="$cmakeflags -DXRT_CLANG_TIDY=ON"
    fi

    cmake -B $BUILDDIR/$config $cmakeflags $BUILDDIR/..
    cmake --build $BUILDDIR/$config --config $config --verbose -j $CORE
    cmake --install $BUILDDIR/$config --config $config --prefix $BUILDDIR/$config/opt/xilinx/aiebu

    if [[ $run_test == "yes" ]]; then
        ctest --test-dir $BUILDDIR/$config -C $config -j $CORE

        if [[ $run_memtest == "yes" ]]; then
            ctest --test-dir $BUILDDIR/$config -C $config -L memcheck -T memcheck
        fi
    fi

    if [[ $config == "Release" ]]; then
        cmake --build $BUILDDIR/$config --config $config --target package --verbose -j $CORE
    fi
}

build_python="yes"
ctrlcode_codegen="no"
usage() { echo "Usage: $0 [-pthc]" 1>&2; exit 1; }

while getopts ":rtphc" o; do
    case "${o}" in
        p)
            build_python="yes"
            ;;
        m)
            run_memtest="yes"
            ;;
        t)
            run_test="no"
            ;;
        c)
            ctrlcode_codegen="yes"
            ;;
        h)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

here=$PWD
cd $BUILDDIR

compile Debug $build_python $ctrlcode_codegen

compile Release $build_python $ctrlcode_codegen

cd $here
