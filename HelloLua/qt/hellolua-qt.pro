#
# Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

TARGET = hellolua
TEMPLATE = app

VERSION = 0.6

INCLUDEPATH += \
    ../Classes

QT += core gui

DEFINES += COCOS2D_DEBUG

COCOSBUILDMODE = static
COCOSMODULES += cocosdenshion lua
include(../../cocos2d-qt.pri)

CCRESOURCES += \
    ../Resources/background.ogg \
    ../Resources/crop.png \
    ../Resources/dog.png \
    ../Resources/effect1.wav \
    ../Resources/farm.jpg \
    ../Resources/hello.lua \
    ../Resources/hello2.lua \
    ../Resources/land.png \
    ../Resources/menu1.png \
    ../Resources/menu2.png \
    ../Resources/fonts/arial.ttf

HEADERS += \
    ../Classes/AppDelegate.h

SOURCES += \
    ../Classes/AppDelegate.cpp \
    main.cpp

symbian {
    TARGET.UID3 = 0xE8AA3F99
    TARGET.CAPABILITY = ReadUserData WriteUserData

    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"

    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x8000000

    ICON = icons/icon.svg

    ccres.sources = $$CCRESOURCES
    ccres.path = !:/private/E8AA3F99/resources
    DEPLOYMENT += ccres
}

contains(MEEGO_EDITION,harmattan) {
    DEFINES += MEEGO_EDITION_HARMATTAN

    target.path = /opt/hellolua/bin
    INSTALLS += target

    ccres.files = $$CCRESOURCES
    ccres.path = /opt/hellolua/resources
    INSTALLS += ccres

    desktop.path = /usr/share/applications
    desktop.files += qtc_packaging/debian_harmattan/$${TARGET}.desktop
    INSTALLS += desktop

    gameclassify.path = /usr/share/policy/etc/syspart.conf.d
    gameclassify.files += qtc_packaging/debian_harmattan/$${TARGET}.conf
    INSTALLS += gameclassify

    icon80.path = /usr/share/icons/hicolor/64x64/apps
    icon80.files += icons/$${TARGET}.png
    INSTALLS += icon80
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
