#!/usr/bin/env bash

set -e

export PATH=/usr/local/opt/llvm/bin:$PATH

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_INSTALL_PREFIX=$HOME/opt/ccls \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES \
    -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_PREFIX_PATH=/usr/local/opt/llvm \
    -DCMAKE_LINKER=ld.ldd \
    -DCMAKE_AR=llvm-ar \
    -DCMAKE_RANLIB=llvm-ranlib
ln -sf build/compile_commands.json
