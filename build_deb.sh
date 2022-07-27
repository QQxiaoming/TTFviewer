#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt6.2.0/6.2.0/gcc_64

# 定义版本号
TTFVIEWER_MAJARVERSION="0"
TTFVIEWER_SUBVERSION="2"
TTFVIEWER_REVISION="0"
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
qmake ./TTFviewer.pro -spec linux-g++ CONFIG+=qtquickcompiler
make clean
make -j8 
# clean打包目录
rm -rf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64 
rm -f ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64.deb
# 构建打包目录
cp -r ./dpkg/TTFviewer ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64
mkdir -p ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer
cp -r ./test ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/test
cp ./build_release/out/TTFviewer ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/TTFviewer
# 使用linuxdeployqt拷贝依赖so库到打包目录
./tools/linuxdeployqt ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/TTFviewer -appimage
# 由于linuxdeployqt目前（截止2022.07.18）还不支持Qt6的plugins依赖分析，这里我们手动拷贝
mkdir ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins
mkdir ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/iconengines
cp $QT_PLUGIN_PATH/iconengines/libqsvgicon.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/iconengines/libqsvgicon.so
mkdir ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats
cp $QT_PLUGIN_PATH/imageformats/libqgif.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqgif.so
cp $QT_PLUGIN_PATH/imageformats/libqico.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqico.so
cp $QT_PLUGIN_PATH/imageformats/libqjpeg.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqjpeg.so
cp $QT_PLUGIN_PATH/imageformats/libqsvg.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/imageformats/libqsvg.so
mkdir ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforminputcontexts
cp $QT_PLUGIN_PATH/platforminputcontexts/libcomposeplatforminputcontextplugin.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforminputcontexts/libcomposeplatforminputcontextplugin.so
cp $QT_PLUGIN_PATH/platforminputcontexts/libibusplatforminputcontextplugin.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforminputcontexts/libibusplatforminputcontextplugin.so
mkdir ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforms
cp $QT_PLUGIN_PATH/platforms/libqxcb.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/platforms/libqxcb.so
mkdir ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/xcbglintegrations
cp $QT_PLUGIN_PATH/xcbglintegrations/libqxcb-egl-integration.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/xcbglintegrations/libqxcb-egl-integration.so
cp $QT_PLUGIN_PATH/xcbglintegrations/libqxcb-glx-integration.so ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/plugins/xcbglintegrations/libqxcb-glx-integration.so
cp $QT_DIR/lib/libQt6XcbQpa.so.6 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/lib/libQt6XcbQpa.so.6
cp $QT_DIR/lib/libQt6OpenGL.so.6 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/lib/libQt6OpenGL.so.6
rm -rf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/doc ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/default.png ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/AppRun ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/default.desktop
cp ./img/ico.png ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/TTFviewer.png
# 配置打包信息
sed -i "s/#VERSION#/$TTFVIEWER_MAJARVERSION.$TTFVIEWER_SUBVERSION$TTFVIEWER_REVISION/g" ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/DEBIAN/control
SIZE=$(du -sh -B 1024 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64 | sed "s/.\///g")
InstalledSize=$SIZE
sed -i "s/#SIZE#/$InstalledSize/g" ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/DEBIAN/control
chmod 755 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/* -R
# 打包
dpkg -b ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64.deb
echo build success!
###############################################################################
