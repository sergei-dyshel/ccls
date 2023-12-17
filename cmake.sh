#!/usr/bin/env bash

set -e

cmake=cmake

cmake_args=(
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
    -DCMAKE_INSTALL_PREFIX=$HOME/opt/ccls
    -DCMAKE_CXX_COMPILER=clang++
    -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON
)

if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "MacOS detected"
    # currently building with system clang (build with brew clang doesn't work)
    # export PATH=$(brew --prefix llvm)/bin:$PATH
    cmake=$(brew --prefix cmake)/bin/cmake
    cmake_args+=(
        # -DCMAKE_LINKER=ld.ldd
        # -DCMAKE_AR=llvm-ar
        # -DCMAKE_RANLIB=llvm-ranlib
        # -DCLANG_RESOURCE_DIR="$(clang++ -print-resource-dir)"
        -DCMAKE_PREFIX_PATH="$(brew --prefix llvm)/lib/cmake"
    )
elif [[ -n "$NIX_STORE" ]]; then
    echo "NIX detected"
    if [[ -f /etc/system-release ]] && grep "Amazon Linux Bare Metal release 2012" /etc/system-release > /dev/null; then
        echo "AL2012 detected"
        cmake_args+=(
            -DCMAKE_CXX_FLAGS=-D__STDC_FORMAT_MACROS
        )
    fi
else
    echo "Environment not detected"
    exit 1
fi


set -x
$cmake ${cmake_args[@]} "$@"
