#ifndef __ADVCOCOS_DENSHION_TEST__
#define __ADVCOCOS_DENSHION_TEST__

#include "../testBasic.h"
#include "AdvancedAudioEngine.h"

using namespace CocosDenshion;

class AdvCocosDenshionTest : public CCLayer, public AudioEventListener
{
public:
    AdvCocosDenshionTest(void);
    ~AdvCocosDenshionTest(void);

    void menuCallback(CCObject * pSender);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void onExit();

public: // AudioEventListener
    void effectEnded(SfxInstanceId soundId);

private:
    CCMenu* m_pItmeMenu;
    CCPoint m_tBeginPos;
    AdvancedAudioEngine *m_audioEngine;
    int m_nTestCount;
    unsigned int m_nActiveSoundId;
    unsigned int m_nSfxId;
    unsigned int m_nMusicSfxId;
    float m_fadeOut;
    float m_fadeIn;
};

class AdvCocosDenshionTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

#endif //__ADVCOCOS_DENSHION_TEST__
