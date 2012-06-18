#
# Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

TARGET = helloworld
TEMPLATE = app

VERSION = 0.8

INCLUDEPATH += \
    ../Classes

QT += core gui

COCOSBUILDMODE = static
include(../../cocos2d-qt.pri)

CCRESOURCES += \
    ../Resources/CloseNormal.png \
    ../Resources/CloseSelected.png \
    ../Resources/fps_images.fnt \
    ../Resources/fps_images.png \
    ../Resources/HelloWorld.png

HEADERS += \
    ../Classes/HelloWorldScene.h \
    ../Classes/AppDelegate.h \
    main.h

SOURCES += \
    ../Classes/HelloWorldScene.cpp \
    ../Classes/AppDelegate.cpp \
    main.cpp

symbian {
    TARGET.UID3 = 0xE8AA3F79
    TARGET.CAPABILITY = ReadUserData WriteUserData

    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"

    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x8000000

    ICON = icons/icon.svg

    ccres.sources = $$CCRESOURCES
    ccres.path = !:/private/E8AA3F79/resources
    DEPLOYMENT += ccres
}

unix {
    target.path = /opt/helloworld/bin
    INSTALLS += target

    ccres.files = $$CCRESOURCES
    ccres.path = /opt/helloworld/resources
    INSTALLS += ccres

    contains(MEEGO_EDITION,harmattan) {
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
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
