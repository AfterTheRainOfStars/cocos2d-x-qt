#
# Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = CocosDenshion/proj.qt/cocosdenshion.pro
SUBDIRS += Box2D/proj.qt/box2d.pro
SUBDIRS += chipmunk/proj.qt/chipmunk.pro
SUBDIRS += cocos2dx/proj.qt/cocos2dx-qt.pro
SUBDIRS += lua/proj.qt/lua.pro

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
