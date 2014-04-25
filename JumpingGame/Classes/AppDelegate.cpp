/**
* Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "AppDelegate.h"

#include "cocos2d.h"
#include "MainMenuScene.h"

#include "CCEGLView.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_QT)
AppDelegate::AppDelegate(int &argc, char** argv) :
    CCApplication(argc, argv)
{
}
#else
AppDelegate::AppDelegate()
{
}
#endif

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::initInstance() {
    bool bRet = false;
    do {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The JumpingGame is designed as HVGA.
        CCEGLView * pMainWnd = new CCEGLView();
        CC_BREAK_IF(! pMainWnd
                || ! pMainWnd->Create(TEXT("cocos2d: Hello World"), 480, 320));

#endif  // CC_PLATFORM_WIN32
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

        // OpenGLView initialized in testsAppDelegate.mm on ios platform, nothing need to do here.

#endif  // CC_PLATFORM_IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        // OpenGLView initialized in JumpingGame/android/jni/jumpinggame/main.cpp
        // the default setting is to create a fullscreen view
        // if you want to use auto-scale, please enable view->create(320,480) in main.cpp
        // if the resources under '/sdcard" or other writeable path, set it.
        // warning: the audio source should in assets/
        // cocos2d::CCFileUtils::setResourcePath("/sdcard");

#endif  // CC_PLATFORM_ANDROID
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)

        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The JumpingGame is designed as HVGA.
        CCEGLView* pMainWnd = new CCEGLView(this);
        CC_BREAK_IF(! pMainWnd || ! pMainWnd->Create(320,480, WM_WINDOW_ROTATE_MODE_CW));

#ifndef _TRANZDA_VM_
        // on wophone emulator, we copy resources files to Work7/NEWPLUS/TDA_DATA/Data/ folder instead of zip file
        cocos2d::CCFileUtils::setResource("JumpingGame.zip");
#endif

#endif  // CC_PLATFORM_WOPHONE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
        // MaxAksenov said it's NOT a very elegant solution. I agree, haha
        CCDirector::sharedDirector()->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The JumpingGame is designed as HVGA.
        CCEGLView * pMainWnd = new CCEGLView();
        CC_BREAK_IF(! pMainWnd
                || ! pMainWnd->Create("cocos2d: Hello World", 480, 320 ,480, 320));

        CCFileUtils::setResourcePath("../Resource/");

#endif  // CC_PLATFORM_LINUX

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BADA)

        CCEGLView * pMainWnd = new CCEGLView();
        CC_BREAK_IF(! pMainWnd|| ! pMainWnd->Create(this, 480, 320));
        pMainWnd->setDeviceOrientation(Osp::Ui::ORIENTATION_LANDSCAPE);
        CCFileUtils::setResourcePath("/Res/");

#endif  // CC_PLATFORM_BADA

#if (CC_TARGET_PLATFORM == CC_PLATFORM_QNX)
        CCEGLView * pMainWnd = new CCEGLView();
        CC_BREAK_IF(! pMainWnd|| ! pMainWnd->Create(480, 320));
        CCFileUtils::setResourcePath("./app/native/Resource");
#endif // CC_PLATFORM_QNX

#if (CC_TARGET_PLATFORM == CC_PLATFORM_QT)

        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The JumpingGame is designed as HVGA.
        CCEGLView * pMainWnd = new CCEGLView(NULL);
        CC_BREAK_IF(! pMainWnd || ! pMainWnd->Create());

#ifdef Q_OS_SYMBIAN
        CCFileUtils::setResourcePath("/private/E8AA3FAF/resources");
#else
        CCFileUtils::setResourcePath("/opt/jumpinggame/resources");
#endif

#endif  // CC_PLATFORM_QT

        bRet = true;
    } while (0);
    return bRet;
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();

    pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayFPS(true);

    pDirector->setDeviceOrientation(kCCDeviceOrientationPortrait);
    //pDirector->setDeviceOrientation(kCCDeviceOrientationPortraitUpsideDown);
    //pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);
    //pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeRight);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainMenu::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->resume();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
