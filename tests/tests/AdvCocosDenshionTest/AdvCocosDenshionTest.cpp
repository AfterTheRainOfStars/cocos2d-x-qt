#include "AdvCocosDenshionTest.h"
#include "cocos2d.h"

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define EFFECT_FILE        "effect2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
    #define EFFECT_FILE        "effect1.raw"
#else
    #define EFFECT_FILE        "effect1.wav"
#endif // CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #define MUSIC_FILE        "music.mid"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_QNX)
    #define MUSIC_FILE        "background.ogg"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_QT)
    #define MUSIC_FILE        "background.ogg"
#else
    #define MUSIC_FILE        "background.mp3"
#endif // CC_PLATFORM_WIN32

using namespace cocos2d;
using namespace CocosDenshion;

#define LINE_SPACE          40

AdvCocosDenshionTest::AdvCocosDenshionTest()
: m_pItmeMenu(NULL),
m_tBeginPos(CCPointZero),
m_nActiveSoundId(0)
{
    std::string testItems[] = {
        "play background music",
        "play effect",
        "play effect repeatly",
        "is playing",
        "get active instance",
        "stop",
        "pause",
        "resume",
        "unload",
        "pause all",
        "resume all",
        "stop all",
        "add master volume",
        "sub master volume",
        "add volume",
        "sub volume",
        "increase pitch",
        "decrease pitch",
        "pan right",
        "pan left",
        "fade out",
        "fade in",
        "no fade",
        "random seek"
    };

    // add menu items for tests
    m_pItmeMenu = CCMenu::menuWithItems(NULL);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_nTestCount = sizeof(testItems) / sizeof(testItems[0]);

    for (int i = 0; i < m_nTestCount; ++i)
    {
        CCLabelTTF* label = CCLabelTTF::labelWithString(testItems[i].c_str(), "Arial", 24);
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::itemWithLabel(label, this,
            menu_selector(AdvCocosDenshionTest::menuCallback));
        
        m_pItmeMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( CCPointMake( s.width / 2, (s.height - (i + 1) * LINE_SPACE) ));
    }

    m_pItmeMenu->setContentSize(CCSizeMake(s.width, (m_nTestCount + 1) * LINE_SPACE));
    m_pItmeMenu->setPosition(CCPointZero);
    addChild(m_pItmeMenu);

    setIsTouchEnabled(true);

    AdvancedAudioEngine::sharedEngine()->setAudioEventListener(this);

    // preload background music and effect
    m_nMusicSfxId = AdvancedAudioEngine::sharedEngine()->loadMusic(
                CCFileUtils::fullPathFromRelativePath(MUSIC_FILE));
    m_nSfxId = AdvancedAudioEngine::sharedEngine()->loadEffect(
                CCFileUtils::fullPathFromRelativePath(EFFECT_FILE));
    
    // set default volume
    AdvancedAudioEngine::sharedEngine()->setVolume(0.5);
}

AdvCocosDenshionTest::~AdvCocosDenshionTest()
{
}

void AdvCocosDenshionTest::onExit()
{
    CCLayer::onExit();

    AdvancedAudioEngine::sharedEngine()->end();
}

void AdvCocosDenshionTest::menuCallback(CCObject * pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    switch(nIdx)
    {
    case 0:
        // play background music
        m_nActiveSoundId = AdvancedAudioEngine::sharedEngine()->play(m_nMusicSfxId);
        AdvancedAudioEngine::sharedEngine()->setLoopCount(m_nActiveSoundId, -1);
        AdvancedAudioEngine::sharedEngine()->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        AdvancedAudioEngine::sharedEngine()->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        AdvancedAudioEngine::sharedEngine()->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 1:
        // play effect
        m_nActiveSoundId = AdvancedAudioEngine::sharedEngine()->play(m_nSfxId);
        AdvancedAudioEngine::sharedEngine()->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        AdvancedAudioEngine::sharedEngine()->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        AdvancedAudioEngine::sharedEngine()->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 2:
        // play effect
        m_nActiveSoundId = AdvancedAudioEngine::sharedEngine()->play(m_nSfxId);
        AdvancedAudioEngine::sharedEngine()->setLoopCount(m_nActiveSoundId, -1);
        AdvancedAudioEngine::sharedEngine()->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        AdvancedAudioEngine::sharedEngine()->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        AdvancedAudioEngine::sharedEngine()->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 3:
        // is playing
        if (AdvancedAudioEngine::sharedEngine()->isPlaying(m_nActiveSoundId))
        {
            CCLOG("is playing");
        }
        else
        {
            CCLOG("not playing");
        }
        break;
    case 4:
        // get active effect
        m_nActiveSoundId = AdvancedAudioEngine::sharedEngine()->getActiveSfxInstanceId();
        break;
    case 5:
        // stop effect
        AdvancedAudioEngine::sharedEngine()->stop(m_nActiveSoundId);
        break;
    case 6:
        // pause effect
        AdvancedAudioEngine::sharedEngine()->pause(m_nActiveSoundId);
        break;
    case 7:
        // resume effect
        AdvancedAudioEngine::sharedEngine()->resume(m_nActiveSoundId);
        break;
    case 8:
        // unload effect
        AdvancedAudioEngine::sharedEngine()->unload(m_nSfxId);
        break;
    case 9:
        // pause all
        AdvancedAudioEngine::sharedEngine()->pauseAll();
        break;
    case 10:
        // resume all
        AdvancedAudioEngine::sharedEngine()->resumeAll();
        break;
    case 11:
        // stop all
        AdvancedAudioEngine::sharedEngine()->stopAll();
        break;
    case 12:
        // add master volume
        AdvancedAudioEngine::sharedEngine()->setVolume(
            AdvancedAudioEngine::sharedEngine()->getVolume() + 0.1f);
        break;
    case 13:
        // sub master volume
        AdvancedAudioEngine::sharedEngine()->setVolume(
            AdvancedAudioEngine::sharedEngine()->getVolume() - 0.1f);
        break;
    case 14:
        // add volume
        AdvancedAudioEngine::sharedEngine()->setVolume(m_nActiveSoundId,
            AdvancedAudioEngine::sharedEngine()->getVolume(m_nActiveSoundId) + 0.1f);
        break;
    case 15:
        // sub volume
        AdvancedAudioEngine::sharedEngine()->setVolume(m_nActiveSoundId,
           AdvancedAudioEngine::sharedEngine()->getVolume(m_nActiveSoundId) - 0.1f);
        break;
    case 16:
        // increase pitch
        AdvancedAudioEngine::sharedEngine()->setPitch(m_nActiveSoundId,
            AdvancedAudioEngine::sharedEngine()->getPitch(m_nActiveSoundId) + 0.1f);
        break;
    case 17:
        // sub pitch
        AdvancedAudioEngine::sharedEngine()->setPitch(m_nActiveSoundId,
            AdvancedAudioEngine::sharedEngine()->getPitch(m_nActiveSoundId) - 0.1f);
        break;
    case 18:
        // increase pan
        AdvancedAudioEngine::sharedEngine()->setPanning(m_nActiveSoundId,
            AdvancedAudioEngine::sharedEngine()->getPanning(m_nActiveSoundId) + 0.1f);
        break;
    case 19:
        // sub pan
        AdvancedAudioEngine::sharedEngine()->setPanning(m_nActiveSoundId,
            AdvancedAudioEngine::sharedEngine()->getPanning(m_nActiveSoundId) - 0.1f);
        break;
    case 20:
        // fade out
        m_fadeOut = 0.5f;
        AdvancedAudioEngine::sharedEngine()->setFadeOutDuration(
            m_nActiveSoundId, m_fadeOut);
        break;
    case 21:
        // fade in
        m_fadeIn = 0.5f;
        AdvancedAudioEngine::sharedEngine()->setFadeInDuration(
            m_nActiveSoundId, m_fadeIn);
        break;
    case 22:
        // no fade
        m_fadeIn = 0.0f;
        m_fadeOut = 0.0f;
        AdvancedAudioEngine::sharedEngine()->setFadeInDuration(
            m_nActiveSoundId, m_fadeIn);
        AdvancedAudioEngine::sharedEngine()->setFadeOutDuration(
            m_nActiveSoundId, m_fadeOut);
        break;
    case 23:
        {
        // random seek
        float pos = (float)rand() / (float)RAND_MAX * 5.0f * 44100;
        CCLOG("Seek to %.3f", pos);
        AdvancedAudioEngine::sharedEngine()->seek(m_nActiveSoundId, pos);
        }
        break;
    }
}

void AdvCocosDenshionTest::effectEnded(SfxInstanceId soundId)
{
    CCLOG("Effect %u ended", soundId);
    m_nActiveSoundId = AdvancedAudioEngine::sharedEngine()->getActiveSfxInstanceId();
}

void AdvCocosDenshionTest::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->locationInView();    
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
}

void AdvCocosDenshionTest::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint touchLocation = touch->locationInView();    
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    float nMoveY = touchLocation.y - m_tBeginPos.y;

    CCPoint curPos  = m_pItmeMenu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (nextPos.y < 0.0f)
    {
        m_pItmeMenu->setPosition(CCPointZero);
        return;
    }

    if (nextPos.y > ((m_nTestCount + 1)* LINE_SPACE - winSize.height))
    {
        m_pItmeMenu->setPosition(ccp(0, ((m_nTestCount + 1)* LINE_SPACE - winSize.height)));
        return;
    }

    m_pItmeMenu->setPosition(nextPos);
    m_tBeginPos = touchLocation;
}

void AdvCocosDenshionTestScene::runThisTest()
{
    CCLayer* pLayer = new AdvCocosDenshionTest();
    addChild(pLayer);
    pLayer->autorelease();

    CCDirector::sharedDirector()->replaceScene(this);
}
