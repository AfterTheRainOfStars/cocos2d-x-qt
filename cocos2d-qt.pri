#
# Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

QMAKE_CFLAGS += -std=c99

DEFINES += SYSTEM_QT

unix {
    QT += core gui xml
    CONFIG += mobility
    MOBILITY += multimedia systeminfo sensors

    contains(MEEGO_EDITION,harmattan) {
        QT += meegographicssystemhelper
        DEFINES += MEEGO_EDITION_HARMATTAN
    }
}

symbian {
    QT += core gui xml
    CONFIG += mobility
    MOBILITY += multimedia systeminfo sensors
    DEFINES += Q_OS_SYMBIAN
}

contains(COCOSBUILDMODE, inc) {
    DEFINES += COCOS2DXQT_LIBRARY

    contains(COCOSMODULES, cocosdenshion) {
        include(CocosDenshion/proj.qt/cocosdenshion.pri)
    }

    contains(COCOSMODULES, box2d) {
        include(Box2D/proj.qt/box2d.pri)
    }

    contains(COCOSMODULES, chipmunk) {
        include(chipmunk/proj.qt/chipmunk.pri)
    }

    contains(COCOSMODULES, lua) {
        include(lua/proj.qt/lua.pri)
    }

    include(cocos2dx/proj.qt/cocos2dx-qt.pri)
} else {
    contains(COCOSBUILDMODE, shared) {
        contains(COCOSMODULES, cocosdenshion) {
            include(CocosDenshion/proj.qt/cocosdenshion.pri)
            LIBS += -lcocosdenshion-qt
        }

        # FIXME: box2d and chipmunk libs do not export anything
        # usefull. The sources are included into the
        # project for now.
        contains(COCOSMODULES, box2d) {
            include(Box2D/proj.qt/box2d.pri)
        }

        contains(COCOSMODULES, chipmunk) {
            include(chipmunk/proj.qt/chipmunk.pri)
        }

        contains(COCOSMODULES, lua) {
            include(lua/proj.qt/lua.pri)
            LIBS += -llua-qt
        }

        include(cocos2dx/proj.qt/cocos2dx-qt.pri)
        LIBS += -lcocos2dx-qt
    } else {
        contains(COCOSBUILDMODE, static) {
            unix {
                contains(COCOSMODULES, cocosdenshion) {
                    include(CocosDenshion/proj.qt/cocosdenshion.pri)
                    LIBS += -l:libcocosdenshion-qt.a
                }

                contains(COCOSMODULES, box2d) {
                    DEFINES += DONT_INCLUDE_BOX2D_SOURCES
                    include(Box2D/proj.qt/box2d.pri)
                    LIBS += -l:libbox2d-qt.a
                }

                contains(COCOSMODULES, chipmunk) {
                    DEFINES += DONT_INCLUDE_CHIPMUNK_SOURCES
                    include(chipmunk/proj.qt/chipmunk.pri)
                    LIBS += -l:libchipmunk-qt.a
                }

                contains(COCOSMODULES, lua) {
                    include(lua/proj.qt/lua.pri)
                    LIBS += -l:liblua-qt.a
                }

                LIBS += -l:libcocos2dx-qt.a
                include(cocos2dx/proj.qt/cocos2dx-qt.pri)
            }

            symbian {
                contains(COCOSMODULES, cocosdenshion) {
                    include(CocosDenshion/proj.qt/cocosdenshion.pri)
                    LIBS += -lcocosdenshion-qt.lib
                }

                contains(COCOSMODULES, box2d) {
                    DEFINES += DONT_INCLUDE_BOX2D_SOURCES
                    include(Box2D/proj.qt/box2d.pri)
                    LIBS += -lbox2d-qt.lib
                }

                contains(COCOSMODULES, chipmunk) {
                    DEFINES += DONT_INCLUDE_CHIPMUNK_SOURCES
                    include(chipmunk/proj.qt/chipmunk.pri)
                    LIBS += -lchipmunk-qt.lib
                }

                contains(COCOSMODULES, lua) {
                    include(lua/proj.qt/lua.pri)
                    LIBS += -llua-qt.lib
                }

                LIBS += -lcocos2dx-qt.lib
                include(cocos2dx/proj.qt/cocos2dx-qt.pri)
            }
        } else {
            error("COCOSBUILDMODE not defined!");
        }
    }
}
