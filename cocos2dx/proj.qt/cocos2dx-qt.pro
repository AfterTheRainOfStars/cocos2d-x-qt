#
# Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

TARGET = cocos2dx-qt
TEMPLATE = lib

QT += gui xml

CONFIG += mobility
MOBILITY += multimedia systeminfo sensors

DEFINES += COCOS2D_DEBUG

include(../../cocos2d-qt-common.pri)
include(cocos2dx-qt.pri)

symbian {
    TARGET.UID3 = 0xE8AB1200
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

contains(MEEGO_EDITION,harmattan) {
    QT += meegographicssystemhelper

    QMAKE_POST_LINK += if [ -e lib$${TARGET}.a ]; then cp lib$${TARGET}.a $$[QT_INSTALL_LIBS]; fi

    target.path = /usr/lib
    INSTALLS += target
}
