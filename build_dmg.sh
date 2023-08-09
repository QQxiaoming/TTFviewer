#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt6.2.0/6.2.0/gcc_64

# 定义版本号
TTFVIEWER_MAJARVERSION="0"
TTFVIEWER_SUBVERSION="2"
TTFVIEWER_REVISION="4"
###############################################################################


###############################################################################
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$QT_DIR/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_DIR/plugins
export QML2_IMPORT_PATH=$QT_DIR/qml
# 合成版本号
TTFVIEWER_VERSION="V"$TTFVIEWER_MAJARVERSION$TTFVIEWER_SUBVERSION$TTFVIEWER_REVISION
# 编译
rm -rf .qmake.stash Makefile
$QT_DIR/bin/lrelease ./TTFviewer.pro
$QT_DIR/bin/qmake -makefile
make
cp -R ./test ./build_release/out/TTFviewer.app/Contents/Resources/test
cp ./tools/create-dmg/build-dmg.sh ./build_release/out/build-dmg.sh
cp ./tools/create-dmg/installer_background.png ./build_release/out/installer_background.png
cd ./build_release/out
# 打包
$QT_DIR/bin/macdeployqt TTFviewer.app
otool -L ./TTFviewer.app/Contents/MacOS/TTFviewer
./build-dmg.sh TTFviewer
echo build success!
###############################################################################
