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

###############################################################################
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
        src/configFile.h

FORMS += \
        src/UI_TTFviewer.ui \
        src/UI_ImgViewer.ui

RESOURCES += \
        src/img.qrc

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

    LIBS += $${FREETPE2_DIR}\lib\libfreetype.a

    VERSION = $${YVYVIEWER_VERSION}.000
    QMAKE_TARGET_PRODUCT = "TTFviewer"
    QMAKE_TARGET_DESCRIPTION = "TTFviewer based on Qt $$[QT_VERSION]"
    QMAKE_TARGET_COPYRIGHT = "GNU General Public License v3.0"

    git_tag.commands = $$quote("cd $$PWD && git describe --always --long --dirty --abbrev=10 --tags | $$PWD/tools/awk/awk.exe \'{print \"\\\"\"\$$0\"\\\"\"}\' > git_tag.inc")
}

unix:!macx:{
    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie

    INCLUDEPATH += -I $${FREETPE2_DIR}/include
    INCLUDEPATH += -I $${FREETPE2_DIR}/include/freetype2
    DEPENDPATH +=$${FREETPE2_DIR}/include
    DEPENDPATH +=$${FREETPE2_DIR}/include/freetype2
    LIBS += -L $${FREETPE2_DIR}/lib/ -lfreetype
    
    git_tag.commands = $$quote("cd $$PWD && git describe --always --long --dirty --abbrev=10 --tags | awk \'{print \"\\\"\"\$$0\"\\\"\"}\' > git_tag.inc")
}

macx:{
    QMAKE_RPATHDIR=$ORIGIN
    
    INCLUDEPATH += -I $${FREETPE2_DIR}/include
    INCLUDEPATH += -I $${FREETPE2_DIR}/include/freetype2
    DEPENDPATH +=$${FREETPE2_DIR}/include
    DEPENDPATH +=$${FREETPE2_DIR}/include/freetype2
    LIBS += -L $${FREETPE2_DIR}/lib/ -lfreetype

    git_tag.commands = $$quote("cd $$PWD && git describe --always --long --dirty --abbrev=10 --tags | awk \'{print \"\\\"\"\$$0\"\\\"\"}\' > git_tag.inc")
}

git_tag.target = $$PWD/git_tag.inc
git_tag.depends = FORCE
PRE_TARGETDEPS += $$PWD/git_tag.inc
QMAKE_EXTRA_TARGETS += git_tag

###############################################################################
