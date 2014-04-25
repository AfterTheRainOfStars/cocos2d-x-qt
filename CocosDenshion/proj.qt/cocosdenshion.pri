#
# Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

INCLUDEPATH += \
    $$PWD/../include \
    $$PWD/../qt/qtgameenabler/src \
    $$PWD/../ \
    $$PWD/../../cocos2dx/platform \
    $$PWD/../../cocos2dx/include \
    $$PWD/../../cocos2dx

contains(DEFINES,COCOS2DXQT_LIBRARY) {
    include($$PWD/../qt/qtgameenabler/qtgameenableraudio.pri)

    SOURCES += \
        $$PWD/../qt/SimpleAudioEngine.cpp \
        $$PWD/../qt/QtAudioPlayer.cpp
}

PUBLIC_HEADERS = \
    $$PWD/../include/SimpleAudioEngine.h \
    $$PWD/../include/Export.h \
    $$PWD/../qt/QtAudioPlayer.h \
    $$PWD/../qt/AudioPlayer.h

HEADERS += $${PUBLIC_HEADERS}

symbian {
    LIBS += -lremconcoreapi -lremconinterfacebase -lmmfdevsound
}
