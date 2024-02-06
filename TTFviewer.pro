###############################################################################
#                                                                             #
# TTFviewer 工程文件                                                           #  
#                                                                             # 
###############################################################################
win32:{
    include(partform_win32.pri)
}

unix:{
    include(partform_unix.pri)
}

# 定义版本号路径
TTFVIEWER_VERSION="$$cat(./version.txt)"

###############################################################################

!versionAtLeast(QT_VERSION, 6.5.0) {
    message("Cannot use Qt $$QT_VERSION")
    error("Use Qt 6.5.0 or newer")
}

# 定义需要的Qt组件
QT       += core gui
QT       += xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 编译配置
TARGET = TTFviewer
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION="\\\"V$${TTFVIEWER_VERSION}\\\""
CONFIG += c++11

# 源文件配置
INCLUDEPATH += \
        $$PWD \
        $$PWD/src 

SOURCES += \
        src/TTFviewer.cpp \
        src/ImgViewer.cpp \
        src/TTFdecoder.cpp \
        src/configFile.cpp

HEADERS += \
        src/TTFviewer.h \
        src/ImgViewer.h \
        src/TTFdecoder.h \
        src/font_to_svg.hpp \
        src/filedialog.h \
        src/configFile.h

FORMS += \
        src/UI_TTFviewer.ui \
        src/UI_ImgViewer.ui

RESOURCES += \
        src/img.qrc

TRANSLATIONS += \
    lang/ttfviewer_zh_CN.ts \
    lang/ttfviewer_ja_JP.ts \
    lang/ttfviewer_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 输出配置
build_type =
CONFIG(debug, debug|release) {
    build_type = build_debug
} else {
    build_type = build_release
}

DESTDIR     = $$build_type/out
OBJECTS_DIR = $$build_type/obj
MOC_DIR     = $$build_type/moc
RCC_DIR     = $$build_type/rcc
UI_DIR      = $$build_type/ui

# 平台配置
win32:{
    INCLUDEPATH += $${FREETPE2_DIR}\include\freetype2 \
                    $${FREETPE2_DIR}\include

    LIBS += $${FREETPE2_DIR}\bin\libfreetype-6.dll

    VERSION = $${TTFVIEWER_VERSION}.000
    RC_ICONS = "img\ico.ico"
    QMAKE_TARGET_PRODUCT = "TTFviewer"
    QMAKE_TARGET_DESCRIPTION = "TTFviewer based on Qt $$[QT_VERSION]"
    QMAKE_TARGET_COPYRIGHT = "GNU General Public License v3.0"

    build_info.commands = $$quote("c:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe -ExecutionPolicy Bypass -NoLogo -NoProfile -File \"$$PWD/tools/generate_info.ps1\" > $$PWD/build_info.inc")
}

unix:!macx:{
    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie

    INCLUDEPATH += -I $${FREETPE2_DIR}/include
    INCLUDEPATH += -I $${FREETPE2_DIR}/include/freetype2
    DEPENDPATH +=$${FREETPE2_DIR}/include
    DEPENDPATH +=$${FREETPE2_DIR}/include/freetype2
    LIBS += -L $${FREETPE2_DIR}/lib/ -lfreetype
    
    build_info.commands = $$quote("cd $$PWD && ./tools/generate_info.sh > build_info.inc")
}

macx:{
    QMAKE_RPATHDIR=$ORIGIN
    ICON = "img\ico.icns"
    
    INCLUDEPATH += -I $${FREETPE2_DIR}/include
    INCLUDEPATH += -I $${FREETPE2_DIR}/include/freetype2
    DEPENDPATH +=$${FREETPE2_DIR}/include
    DEPENDPATH +=$${FREETPE2_DIR}/include/freetype2
    LIBS += -L $${FREETPE2_DIR}/lib/ -lfreetype

    build_info.commands = $$quote("cd $$PWD && ./tools/generate_info.sh > build_info.inc")
}

build_info.target = $$PWD/build_info.inc
build_info.depends = FORCE
PRE_TARGETDEPS += $$PWD/build_info.inc
QMAKE_EXTRA_TARGETS += build_info

###############################################################################
