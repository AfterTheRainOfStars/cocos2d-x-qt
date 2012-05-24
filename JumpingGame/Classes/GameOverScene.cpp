#include "GameOverScene.h"
#include "MainMenuScene.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

USING_NS_CC;

CCScene* GameOver::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();

    // 'layer' is an autorelease object
    GameOver *layer = GameOver::node();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    initBackground();

    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);

    return true;
}

bool GameOver::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();

    // Change the game state to MainMenu
    CCScene *pScene = MainMenu::scene();
    CCDirector::sharedDirector()->replaceScene(pScene);

    return true;
}

void GameOver::initBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Load the background image sprite and set the position to bottom left side of the screen.
    CCSprite* background = CCSprite::spriteWithFile("gameover.png");
    background->setAnchorPoint(ccp(0.0f, 0.0f));
    background->setPosition(ccp(0.0f, 0.0f));

    // Scale background to cover the whole window.
    CCRect bgRect = background->getTextureRect();
    background->setScaleX(winSize.width / bgRect.size.width);
    background->setScaleY(winSize.height / bgRect.size.height);

    // Add the background as a child to GameOver layer.
    this->addChild(background, 0);
}

