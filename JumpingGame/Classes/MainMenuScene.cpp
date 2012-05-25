/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "MainMenuScene.h"
#include "JumpingGameScene.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

USING_NS_CC;

CCScene* MainMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();

    // 'layer' is an autorelease object
    MainMenu *layer = MainMenu::node();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    initMenu();
    initBackground();

    return true;
}

void MainMenu::menuPlayCallback(CCObject* pSender)
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();

    // Change the game state to JumpingGame
    CCScene *pScene = JumpingGame::scene();
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void MainMenu::menuExitCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainMenu::initMenu()
{
    // create menu items
    CCMenuItemImage* logoItem = CCMenuItemImage::itemFromNormalImage(
                                        "logo.png",
                                        "logo.png",
                                        this,
                                        NULL);
    logoItem->setScale(0.5f);
    logoItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f,
                              CCDirector::sharedDirector()->getWinSize().height * 0.5f + 200));

    CCMenuItemImage *playItem = CCMenuItemImage::itemFromNormalImage(
                                        "button_play.png",
                                        "button_play_down.png",
                                        this,
                                        menu_selector(MainMenu::menuPlayCallback));
    playItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f,
                              CCDirector::sharedDirector()->getWinSize().height * 0.5f + 20));

    CCMenuItemImage *exitItem = CCMenuItemImage::itemFromNormalImage(
                                        "button_exit.png",
                                        "button_exit_down.png",
                                        this,
                                        menu_selector(MainMenu::menuExitCallback) );
    exitItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f,
                              CCDirector::sharedDirector()->getWinSize().height * 0.5f - 120));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::menuWithItems(logoItem, playItem, exitItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
}

void MainMenu::initBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Load the background image sprite and set the position to bottom left side of the screen.
    CCSprite* background = CCSprite::spriteWithFile("background.png");
    background->setAnchorPoint(ccp(0.0f, 0.0f));
    background->setPosition(ccp(0.0f, 0.0f));

    // Scale background to cover the whole window.
    CCRect bgRect = background->getTextureRect();
    background->setScaleX(winSize.width / bgRect.size.width);
    background->setScaleY(winSize.height / bgRect.size.height);

    // Add the background as a child to MainMenu layer.
    this->addChild(background, 0);
}

