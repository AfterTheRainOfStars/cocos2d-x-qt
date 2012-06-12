#
# Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

TARGET = lua-qt
TEMPLATE = lib

QT += gui xml
CONFIG += mobility
MOBILITY += multimedia systeminfo sensors

include(../../cocos2d-qt-common.pri)
include(../../cocos2dx/proj.qt/cocos2dx-qt.pri)
include(../../CocosDenshion/proj.qt/cocosdenshion.pri)
include(lua.pri)

symbian {
    TARGET.UID3 = 0xE8AB1202
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1

    MMP_RULES += EXPORTUNFROZEN
    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"
    MMP_RULES += "OPTION gcce -fno-use-cxa-atexit"

    addFiles.sources = $${TARGET}.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix {
    QT += meegographicssystemhelper

    QMAKE_POST_LINK += if [ -e lib$${TARGET}.a ]; then cp lib$${TARGET}.a $$[QT_INSTALL_LIBS]; fi

    target.path = /usr/lib
    INSTALLS += target
}
