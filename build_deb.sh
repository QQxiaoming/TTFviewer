#!/bin/sh

###############################################################################
# 定义Qt目录
QT_DIR=/opt/Qt6.2.0/6.2.0/gcc_64

# 定义版本号
TTFVIEWER_MAJARVERSION="0"
TTFVIEWER_SUBVERSION="2"
TTFVIEWER_REVISION="5"
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
lrelease ./TTFviewer.pro
qmake ./TTFviewer.pro -spec linux-g++ CONFIG+=qtquickcompiler
make clean
make -j8 
# clean打包目录
rm -rf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64 
rm -f ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64.deb
# 构建打包目录
cp -r ./dpkg/TTFviewer ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64
# 使用linuxdeployqt拷贝依赖so库到打包目录
export QMAKE=$QT_DIR/bin/qmake
./tools/linuxdeploy-x86_64.AppImage --executable=./build_release/out/TTFviewer --appdir=./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt --plugin=qt
rm -rf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/apprun-hooks
mv ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/usr ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer
mv ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/bin/TTFviewer ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/TTFviewer
mv ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/bin/qt.conf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/qt.conf
rm -rf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/bin
sed -i "s/Prefix = ..\//Prefix = .\//g" ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/qt.conf
chrpath -r "\$ORIGIN/./lib" ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/TTFviewer
rm -rf ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/share
cp ./img/ico.png ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/TTFviewer.png
mkdir -p ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer
cp -r ./test ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/opt/TTFviewer/test
# 配置打包信息
sed -i "s/#VERSION#/$TTFVIEWER_MAJARVERSION.$TTFVIEWER_SUBVERSION$TTFVIEWER_REVISION/g" ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/DEBIAN/control
cd ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64
SIZE=$(du -sh -B 1024 ./ | sed "s/.\///g")
cd -
InstalledSize=$SIZE
sed -i "s/#SIZE#/$InstalledSize/g" ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/DEBIAN/control
chmod 755 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64/* -R
# 打包
dpkg -b ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64 ./dpkg/TTFviewer_Linux_"$TTFVIEWER_VERSION"_x86_64.deb
echo build success!
###############################################################################
