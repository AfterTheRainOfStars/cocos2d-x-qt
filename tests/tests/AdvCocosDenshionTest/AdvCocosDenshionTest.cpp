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
      m_audioEngine(NULL),
      m_nActiveSoundId(0),
      m_nSfxId(0),
      m_nMusicSfxId(0),
      m_fadeOut(0.0f),
      m_fadeIn(0.0f)
{
    std::string testItems[] = {
        "play background music",
        "play background music repeatedly",
        "play effect",
        "play effect repeatedly",
        "is playing",
        "is paused",
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

    m_audioEngine = AdvancedAudioEngine::sharedEngine();

    m_audioEngine->setAudioEventListener(this);

    // preload background music and effect
    m_nMusicSfxId = m_audioEngine->loadMusic(
                CCFileUtils::fullPathFromRelativePath(MUSIC_FILE));
    m_nSfxId = m_audioEngine->loadEffect(
                CCFileUtils::fullPathFromRelativePath(EFFECT_FILE));
    
    // set default volume
    m_audioEngine->setVolume(0.5);
}

AdvCocosDenshionTest::~AdvCocosDenshionTest()
{
    m_audioEngine->removeAudioEventListener();
}

void AdvCocosDenshionTest::onExit()
{
    CCLayer::onExit();

    m_audioEngine->removeAudioEventListener();
    m_audioEngine->end();
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
        m_nActiveSoundId = m_audioEngine->play(m_nMusicSfxId);
        m_audioEngine->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        m_audioEngine->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        m_audioEngine->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 1:
        // play background music repeatedly
        m_nActiveSoundId = m_audioEngine->play(m_nMusicSfxId);
        m_audioEngine->setLoopCount(m_nActiveSoundId, -1);
        m_audioEngine->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        m_audioEngine->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        m_audioEngine->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 2:
        // play effect
        m_nActiveSoundId = m_audioEngine->play(m_nSfxId);
        m_audioEngine->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        m_audioEngine->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        m_audioEngine->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 3:
        // play effect repeatedly
        m_nActiveSoundId = m_audioEngine->play(m_nSfxId);
        m_audioEngine->setLoopCount(m_nActiveSoundId, -1);
        m_audioEngine->setFadeInDuration(m_nActiveSoundId, m_fadeIn);
        m_audioEngine->setFadeOutDuration(m_nActiveSoundId, m_fadeOut);
        m_audioEngine->setVolume(m_nActiveSoundId, 0.5f);
        break;
    case 4:
        // is playing
        if (m_audioEngine->isPlaying(m_nActiveSoundId))
        {
            CCLOG("is playing");
        }
        else
        {
            CCLOG("not playing");
        }
        break;
    case 5:
        // is paused
        if (m_audioEngine->isPaused(m_nActiveSoundId))
        {
            CCLOG("is paused");
        }
        else
        {
            CCLOG("not paused");
        }
        break;
    case 6:
        // get active effect
        m_nActiveSoundId = m_audioEngine->getActiveSfxInstanceId();
        CCLOG("active instance %u", m_nActiveSoundId);
        break;
    case 7:
        // stop effect
        m_audioEngine->stop(m_nActiveSoundId);
        break;
    case 8:
        // pause effect
        m_audioEngine->pause(m_nActiveSoundId);
        break;
    case 9:
        // resume effect
        m_audioEngine->resume(m_nActiveSoundId);
        break;
    case 10:
        // unload effect
        m_audioEngine->unload(m_audioEngine->sfxIdForInstance(m_nActiveSoundId));
        break;
    case 11:
        // pause all
        m_audioEngine->pauseAll();
        break;
    case 12:
        // resume all
        m_audioEngine->resumeAll();
        break;
    case 13:
        // stop all
        m_audioEngine->stopAll();
        break;
    case 14:
        // add master volume
        m_audioEngine->setVolume(
            m_audioEngine->getVolume() + 0.1f);
        break;
    case 15:
        // sub master volume
        m_audioEngine->setVolume(
            m_audioEngine->getVolume() - 0.1f);
        break;
    case 16:
        // add volume
        m_audioEngine->setVolume(m_nActiveSoundId,
            m_audioEngine->getVolume(m_nActiveSoundId) + 0.1f);
        break;
    case 17:
        // sub volume
        m_audioEngine->setVolume(m_nActiveSoundId,
           m_audioEngine->getVolume(m_nActiveSoundId) - 0.1f);
        break;
    case 18:
        // increase pitch
        m_audioEngine->setPitch(m_nActiveSoundId,
            m_audioEngine->getPitch(m_nActiveSoundId) + 0.1f);
        break;
    case 19:
        // sub pitch
        m_audioEngine->setPitch(m_nActiveSoundId,
            m_audioEngine->getPitch(m_nActiveSoundId) - 0.1f);
        break;
    case 20:
        // pan right
        m_audioEngine->setPanning(m_nActiveSoundId,
            m_audioEngine->getPanning(m_nActiveSoundId) + 0.1f);
        break;
    case 21:
        // pan left
        m_audioEngine->setPanning(m_nActiveSoundId,
            m_audioEngine->getPanning(m_nActiveSoundId) - 0.1f);
        break;
    case 22:
        // fade out
        m_fadeOut = 0.5f;
        m_audioEngine->setFadeOutDuration(
            m_nActiveSoundId, m_fadeOut);
        break;
    case 23:
        // fade in
        m_fadeIn = 0.5f;
        m_audioEngine->setFadeInDuration(
            m_nActiveSoundId, m_fadeIn);
        break;
    case 24:
        // no fade
        m_fadeIn = 0.0f;
        m_fadeOut = 0.0f;
        m_audioEngine->setFadeInDuration(
            m_nActiveSoundId, m_fadeIn);
        m_audioEngine->setFadeOutDuration(
            m_nActiveSoundId, m_fadeOut);
        break;
    case 25:
        {
        // random seek
        float pos = (float)rand() / (float)RAND_MAX * 5.0f;
        m_audioEngine->seek(m_nActiveSoundId, pos);
        CCLOG("Seek to %.3f (%.3f)", m_audioEngine->position(m_nActiveSoundId),
              pos);
        }
        break;
    }
}

void AdvCocosDenshionTest::effectEnded(SfxInstanceId soundId)
{
    CCLOG("Sound %u ended", soundId);
    m_nActiveSoundId = m_audioEngine->getActiveSfxInstanceId();
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
        m_pItmeMenu->setPosition(ccp(0, ((m_nTestCount + 1)* LINE_SPACE
                                         - winSize.height)));
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
