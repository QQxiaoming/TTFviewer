#!/bin/bash
set -e

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
tar -xzvf freetype-2.13.2.tar.gz
cd $SHELL_FOLDER/freetype-2.13.2
./configure --prefix=/usr/local/ttfviewer \
    --disable-shared \
    --without-zlib \
    --without-png \
    --without-bzip2 \
    --without-harfbuzz \
    --without-brotli \
    --without-librsvg
make all
make install
rm -rf $SHELL_FOLDER/freetype-2.13.2
