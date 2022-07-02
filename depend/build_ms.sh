#!/bin/bash
set -e

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
tar -xzvf freetype-2.3.5.tar.gz
cd $SHELL_FOLDER/freetype-2.3.5
./configure \
    --prefix=/d/freetype \
    --disable-shared \
    --without-zlib
make -j4
make install
