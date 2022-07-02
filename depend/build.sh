#!/bin/bash
set -e

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
tar -xzvf freetype-2.3.5.tar.gz
cd $SHELL_FOLDER/freetype-2.3.5
./configure --prefix=/usr/local/ttfviewer --disable-shared --without-zlib
make -j4
make install
rm -rf $SHELL_FOLDER/freetype-2.3.5
