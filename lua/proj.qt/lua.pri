#
# Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

INCLUDEPATH += \
    $$PWD/../lua \
    $$PWD/../tolua \
    $$PWD/../cocos2dx_support

contains(DEFINES,COCOS2DXQT_LIBRARY) {
    SOURCES += \
        $$PWD/../lua/lgc.h \
        $$PWD/../lua/print.c \
        $$PWD/../lua/lcode.c \
        $$PWD/../lua/lparser.c \
        $$PWD/../lua/lstring.c \
        $$PWD/../lua/lstate.c \
        $$PWD/../lua/ltable.c \
        $$PWD/../lua/ldblib.c \
        $$PWD/../lua/lobject.h \
        $$PWD/../lua/lstrlib.c \
        $$PWD/../lua/ldebug.c \
        $$PWD/../lua/lapi.c \
        $$PWD/../lua/ldo.c \
        $$PWD/../lua/ltm.c \
        $$PWD/../lua/liolib.c \
        $$PWD/../lua/lua.c \
        $$PWD/../lua/lmathlib.c \
        $$PWD/../lua/lopcodes.c \
        $$PWD/../lua/lgc.c \
        $$PWD/../lua/lobject.c \
        $$PWD/../lua/lbaselib.c \
        $$PWD/../lua/lzio.c \
        $$PWD/../lua/lopcodes.h \
        $$PWD/../lua/lauxlib.c \
        $$PWD/../lua/ldump.c \
        $$PWD/../lua/linit.c \
        $$PWD/../lua/loadlib.c \
        $$PWD/../lua/luaconf.h \
        $$PWD/../lua/lcode.h \
        $$PWD/../lua/loslib.c \
        $$PWD/../lua/lfunc.h \
        $$PWD/../lua/lmem.c \
        $$PWD/../lua/ltablib.c \
        $$PWD/../lua/lundump.c \
        $$PWD/../lua/lvm.c \
        $$PWD/../lua/llex.c \
        $$PWD/../lua/lfunc.c \
        $$PWD/../tolua/tolua_to.c \
        $$PWD/../tolua/tolua_push.c \
        $$PWD/../tolua/tolua_map.c \
        $$PWD/../tolua/tolua_is.c \
        $$PWD/../tolua/tolua_event.c \
        $$PWD/../cocos2dx_support/tolua_fix.c \
        $$PWD/../cocos2dx_support/LuaCocos2d.cpp \
        $$PWD/../cocos2dx_support/Cocos2dxLuaLoader.cpp \
        $$PWD/../cocos2dx_support/CCLuaEngine.cpp
}

PUBLIC_HEADERS = \
    $$PWD/../lua/lzio.h \
    $$PWD/../lua/lgc.h \
    $$PWD/../lua/lobject.h \
    $$PWD/../lua/lua.h \
    $$PWD/../lua/lstate.h \
    $$PWD/../lua/ldo.h \
    $$PWD/../lua/lmem.h \
    $$PWD/../lua/lapi.h \
    $$PWD/../lua/lualib.h \
    $$PWD/../lua/lundump.h \
    $$PWD/../lua/llex.h \
    $$PWD/../lua/lmathlib.c \
    $$PWD/../lua/lparser.h \
    $$PWD/../lua/llimits.h \
    $$PWD/../lua/lstring.h \
    $$PWD/../lua/lopcodes.h \
    $$PWD/../lua/lauxlib.h \
    $$PWD/../lua/ltm.h \
    $$PWD/../lua/ldebug.h \
    $$PWD/../lua/luaconf.h \
    $$PWD/../lua/lcode.h \
    $$PWD/../lua/lfunc.h \
    $$PWD/../lua/ltable.h \
    $$PWD/../lua/lvm.h \
    $$PWD/../tolua/tolua++.h \
    $$PWD/../tolua/tolua_event.h \
    $$PWD/../cocos2dx_support/tolua_fix.h \
    $$PWD/../cocos2dx_support/LuaCocos2d.h \
    $$PWD/../cocos2dx_support/Cocos2dxLuaLoader.h \
    $$PWD/../cocos2dx_support/CCLuaEngine.h

HEADERS += $${PUBLIC_HEADERS}
