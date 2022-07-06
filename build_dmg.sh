#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt5.12.2/5.12.2/gcc_64

# 定义版本号
TTFVIEWER_MAJARVERSION="0"
TTFVIEWER_SUBVERSION="1"
TTFVIEWER_REVISION="0"
###############################################################################


###############################################################################
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$QT_DIR/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_DIR/plugins:$QT_PLUGIN_PATH
export QML2_IMPORT_PATH=$QT_DIR/qml:$QML2_IMPORT_PATH
# 合成版本号
TTFVIEWER_VERSION="V"$TTFVIEWER_MAJARVERSION$TTFVIEWER_SUBVERSION$TTFVIEWER_REVISION
# 编译
rm -rf .qmake.stash Makefile
qmake -makefile
make
cp -R ./test ./build_release/out/TTFviewer.app/contents/resources/test
cd ./build_release/out
# 打包
macdeployqt TTFviewer.app
otool -L ./TTFviewer.app/Contents/MacOS/TTFviewer
macdeployqt TTFviewer.app -dmg -verbose=2
echo build success!
###############################################################################
