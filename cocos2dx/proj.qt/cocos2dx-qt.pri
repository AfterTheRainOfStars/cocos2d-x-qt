#
# Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

INCLUDEPATH += \
    $$PWD \
    $$PWD/../ \
    $$PWD/../include \
    $$PWD/../platform \
    $$PWD/../platform/qt \
    $$PWD/../support

contains(MEEGO_EDITION,harmattan) {
    INCLUDEPATH += \
        /usr/include \
        /usr/include/libxml2
}

contains(DEFINES,COCOS2DXQT_LIBRARY) {
    SOURCES += \
        $$PWD/../actions/CCActionTiledGrid.cpp \
        $$PWD/../actions/CCActionProgressTimer.cpp \
        $$PWD/../actions/CCActionPageTurn3D.cpp \
        $$PWD/../actions/CCActionManager.cpp \
        $$PWD/../actions/CCActionInterval.cpp \
        $$PWD/../actions/CCActionInstant.cpp \
        $$PWD/../actions/CCActionGrid3D.cpp \
        $$PWD/../actions/CCActionGrid.cpp \
        $$PWD/../actions/CCActionEase.cpp \
        $$PWD/../actions/CCActionCamera.cpp \
        $$PWD/../actions/CCAction.cpp \
        $$PWD/../base_nodes/CCNode.cpp \
        $$PWD/../base_nodes/CCAtlasNode.cpp \
        $$PWD/../cocoa/CCZone.cpp \
        $$PWD/../cocoa/CCSet.cpp \
        $$PWD/../cocoa/CCObject.cpp \
        $$PWD/../cocoa/CCNS.cpp \
        $$PWD/../cocoa/CCGeometry.cpp \
        $$PWD/../cocoa/CCData.cpp \
        $$PWD/../cocoa/CCAutoreleasePool.cpp \
        $$PWD/../cocoa/CCAffineTransform.cpp \
        $$PWD/../effects/CCGrid.cpp \
        $$PWD/../effects/CCGrabber.cpp \
        $$PWD/../keypad_dispatcher/CCKeypadDispatcher.cpp \
        $$PWD/../keypad_dispatcher/CCKeypadDelegate.cpp \
        $$PWD/../label_nodes/CCLabelTTF.cpp \
        $$PWD/../label_nodes/CCLabelBMFont.cpp \
        $$PWD/../label_nodes/CCLabelAtlas.cpp \
        $$PWD/../layers_scenes_transitions_nodes/CCTransitionRadial.cpp \
        $$PWD/../layers_scenes_transitions_nodes/CCTransitionPageTurn.cpp \
        $$PWD/../layers_scenes_transitions_nodes/CCTransition.cpp \
        $$PWD/../layers_scenes_transitions_nodes/CCScene.cpp \
        $$PWD/../layers_scenes_transitions_nodes/CCLayer.cpp \
        $$PWD/../menu_nodes/CCMenuItem.cpp \
        $$PWD/../menu_nodes/CCMenu.cpp \
        $$PWD/../misc_nodes/CCRibbon.cpp \
        $$PWD/../misc_nodes/CCRenderTexture.cpp \
        $$PWD/../misc_nodes/CCProgressTimer.cpp \
        $$PWD/../misc_nodes/CCMotionStreak.cpp \
        $$PWD/../particle_nodes/CCParticleSystemQuad.cpp \
        $$PWD/../particle_nodes/CCParticleSystemPoint.cpp \
        $$PWD/../particle_nodes/CCParticleSystem.cpp \
        $$PWD/../particle_nodes/CCParticleExamples.cpp \
        $$PWD/../platform/platform.cpp \
        $$PWD/../platform/CCThread.cpp \
        $$PWD/../platform/CCStdC.cpp \
        $$PWD/../platform/CCImage.cpp \
        $$PWD/../platform/CCGL.cpp \
        $$PWD/../platform/CCCommon.cpp \
        $$PWD/../platform/CCFileUtils.cpp \
        $$PWD/../script_support/CCScriptSupport.cpp \
        $$PWD/../sprite_nodes/CCSpriteFrameCache.cpp \
        $$PWD/../sprite_nodes/CCSpriteFrame.cpp \
        $$PWD/../sprite_nodes/CCSpriteBatchNode.cpp \
        $$PWD/../sprite_nodes/CCSprite.cpp \
        $$PWD/../sprite_nodes/CCAnimationCache.cpp \
        $$PWD/../sprite_nodes/CCAnimation.cpp \
        $$PWD/../support/TransformUtils.cpp \
        $$PWD/../support/ccUtils.cpp \
        $$PWD/../support/CCUserDefault.cpp \
        $$PWD/../support/CCProfiling.cpp \
        $$PWD/../support/CCPointExtension.cpp \
        $$PWD/../support/CCArray.cpp \
        $$PWD/../support/base64.cpp \
        $$PWD/../support/image_support/TGAlib.cpp \
        $$PWD/../support/zip_support/ZipUtils.cpp \
        $$PWD/../support/zip_support/unzip.cpp \
        $$PWD/../support/zip_support/ioapi.cpp \
        $$PWD/../text_input_node/CCTextFieldTTF.cpp \
        $$PWD/../text_input_node/CCIMEDispatcher.cpp \
        $$PWD/../textures/CCTexturePVR.cpp \
        $$PWD/../textures/CCTextureCache.cpp \
        $$PWD/../textures/CCTextureAtlas.cpp \
        $$PWD/../textures/CCTexture2D.cpp \
        $$PWD/../tileMap_parallax_nodes/CCTMXXMLParser.cpp \
        $$PWD/../tileMap_parallax_nodes/CCTMXTiledMap.cpp \
        $$PWD/../tileMap_parallax_nodes/CCTMXObjectGroup.cpp \
        $$PWD/../tileMap_parallax_nodes/CCTMXLayer.cpp \
        $$PWD/../tileMap_parallax_nodes/CCTileMapAtlas.cpp \
        $$PWD/../tileMap_parallax_nodes/CCParallaxNode.cpp \
        $$PWD/../touch_dispatcher/CCTouchHandler.cpp \
        $$PWD/../touch_dispatcher/CCTouchDispatcher.cpp \
        $$PWD/../cocos2d.cpp \
        $$PWD/../CCScheduler.cpp \
        $$PWD/../CCDrawingPrimitives.cpp \
        $$PWD/../CCDirector.cpp \
        $$PWD/../CCConfiguration.cpp \
        $$PWD/../CCCamera.cpp \
        $$PWD/../extensions/CCNotificationCenter.cpp

    SOURCES += \
        $$PWD/../platform/qt/CCImage_qt.cpp \
        $$PWD/../platform/qt/CCFileUtils_qt.cpp \
        $$PWD/../platform/qt/CCEGLView_qt.cpp \
        $$PWD/../platform/qt/CCApplication_qt.cpp \
        $$PWD/../platform/qt/CCAccelerometer_qt.cpp \
        $$PWD/../platform/qt/accelerometerfilter.cpp \
        $$PWD/../platform/qt/CCSAXParser_qt.cpp
}

PUBLIC_HEADERS = \
    $$PWD/../include/cocos2d.h \
    $$PWD/../include/CCZone.h \
    $$PWD/../include/CCUserDefault.h \
    $$PWD/../include/ccTypes.h \
    $$PWD/../include/CCTransitionRadial.h \
    $$PWD/../include/CCTransitionPageTurn.h \
    $$PWD/../include/CCTransition.h \
    $$PWD/../include/CCTouchHandler.h \
    $$PWD/../include/CCTouchDispatcher.h \
    $$PWD/../include/CCTouchDelegateProtocol.h \
    $$PWD/../include/CCTouch.h \
    $$PWD/../include/CCTMXXMLParser.h \
    $$PWD/../include/CCTMXTiledMap.h \
    $$PWD/../include/CCTMXObjectGroup.h \
    $$PWD/../include/CCTMXLayer.h \
    $$PWD/../include/CCTileMapAtlas.h \
    $$PWD/../include/CCTexturePVR.h \
    $$PWD/../include/CCTextureCache.h \
    $$PWD/../include/CCTextureAtlas.h \
    $$PWD/../include/CCTexture2D.h \
    $$PWD/../include/CCTextFieldTTF.h \
    $$PWD/../include/CCString.h \
    $$PWD/../include/CCSpriteFrameCache.h \
    $$PWD/../include/CCSpriteFrame.h \
    $$PWD/../include/CCSpriteBatchNode.h \
    $$PWD/../include/CCSprite.h \
    $$PWD/../include/CCSet.h \
    $$PWD/../include/CCScriptSupport.h \
    $$PWD/../include/CCScheduler.h \
    $$PWD/../include/CCScene.h \
    $$PWD/../include/CCRibbon.h \
    $$PWD/../include/CCRenderTexture.h \
    $$PWD/../include/CCProtocols.h \
    $$PWD/../include/CCProgressTimer.h \
    $$PWD/../include/CCPointExtension.h \
    $$PWD/../include/CCParticleSystemQuad.h \
    $$PWD/../include/CCParticleSystemPoint.h \
    $$PWD/../include/CCParticleSystem.h \
    $$PWD/../include/CCParticleExamples.h \
    $$PWD/../include/CCParallaxNode.h \
    $$PWD/../include/CCObject.h \
    $$PWD/../include/CCNode.h \
    $$PWD/../include/CCMutableDictionary.h \
    $$PWD/../include/CCMutableArray.h \
    $$PWD/../include/CCMotionStreak.h \
    $$PWD/../include/CCMenuItem.h \
    $$PWD/../include/CCMenu.h \
    $$PWD/../include/ccMacros.h \
    $$PWD/../include/CCLayer.h \
    $$PWD/../include/CCLabelTTF.h \
    $$PWD/../include/CCLabelBMFont.h \
    $$PWD/../include/CCLabelAtlas.h \
    $$PWD/../include/CCKeypadDispatcher.h \
    $$PWD/../include/CCKeypadDelegate.h \
    $$PWD/../include/CCIMEDispatcher.h \
    $$PWD/../include/CCIMEDelegate.h \
    $$PWD/../include/CCGL.h \
    $$PWD/../include/CCGeometry.h \
    $$PWD/../include/CCEGLView.h \
    $$PWD/../include/CCDrawingPrimitives.h \
    $$PWD/../include/CCDirector.h \
    $$PWD/../include/CCData.h \
    $$PWD/../include/ccConfig.h \
    $$PWD/../include/CCCamera.h \
    $$PWD/../include/CCAutoreleasePool.h \
    $$PWD/../include/CCAtlasNode.h \
    $$PWD/../include/CCArray.h \
    $$PWD/../include/CCApplication.h \
    $$PWD/../include/CCAnimationCache.h \
    $$PWD/../include/CCAnimation.h \
    $$PWD/../include/CCAffineTransform.h \
    $$PWD/../include/CCActionTiledGrid.h \
    $$PWD/../include/CCActionProgressTimer.h \
    $$PWD/../include/CCActionPageTurn3D.h \
    $$PWD/../include/CCActionManager.h \
    $$PWD/../include/CCActionInterval.h \
    $$PWD/../include/CCActionInstant.h \
    $$PWD/../include/CCActionGrid3D.h \
    $$PWD/../include/CCActionGrid.h \
    $$PWD/../include/CCActionEase.h \
    $$PWD/../include/CCActionCamera.h \
    $$PWD/../include/CCAction.h \
    $$PWD/../include/CCAccelerometerDelegate.h \
    $$PWD/../include/CCAccelerometer.h \
    $$PWD/../CCConfiguration.h \
    $$PWD/../support/CCProfiling.h

EXTENSION_HEADERS = \
    $$PWD/../extensions/CCNotificationCenter.h

PLATFORM_HEADERS = \
    $$PWD/../platform/CCPlatformConfig.h \
    $$PWD/../platform/CCPlatformMacros.h \
    $$PWD/../platform/CCCommon.h \
    $$PWD/../platform/platform.h \
    $$PWD/../platform/CCThread.h \
    $$PWD/../platform/CCStdC.h \
    $$PWD/../platform/CCSAXParser.h \
    $$PWD/../platform/CCLibxml2.h \
    $$PWD/../platform/CCImage.h \
    $$PWD/../platform/CCGL.h \
    $$PWD/../platform/CCFileUtils.h \
    $$PWD/../platform/CCEGLView_platform.h \
    $$PWD/../platform/CCArchOptimalParticleSystem.h \
    $$PWD/../platform/CCApplication_platform.h \
    $$PWD/../platform/CCAccelerometer_platform.h

PLATFORM_QT_HEADERS = \
    $$PWD/../platform/qt/CCEGLView_qt.h \
    $$PWD/../platform/qt/CCApplication_qt.h \
    $$PWD/../platform/qt/CCAccelerometer_qt.h \
    $$PWD/../platform/qt/accelerometerfilter.h

DATA_SUPPORT_HEADERS = \
    $$PWD/../support/data_support/utlist.h \
    $$PWD/../support/data_support/uthash.h \
    $$PWD/../support/data_support/ccCArray.h

HEADERS += $${PUBLIC_HEADERS}
HEADERS += $${EXTENSION_HEADERS}
HEADERS += $${PLATFORM_HEADERS}
HEADERS += $${DATA_SUPPORT_HEADERS}

contains(DEFINES,COCOS2DXQT_LIBRARY) {
    HEADERS += $${PLATFORM_QT_HEADERS}
}

HEADERS += \
    $$PWD/../effects/CCGrid.h \
    $$PWD/../effects/CCGrabber.h \
    $$PWD/../cocoa/CCNS.h \
    $$PWD/../support/TransformUtils.h \
    $$PWD/../support/ccUtils.h \
    $$PWD/../support/base64.h \
    $$PWD/../support/image_support/TGAlib.h \
    $$PWD/../support/zip_support/ZipUtils.h \
    $$PWD/../support/zip_support/unzip.h \
    $$PWD/../support/zip_support/ioapi.h

contains(MEEGO_EDITION,harmattan) {
    LIBS += -lpthread -lxml2 -lEGL -lGLES_CM
}

symbian {
    LIBS += -llibEGL -llibgles_cm
    LIBS += -lcone -leikcore -lavkon
}
