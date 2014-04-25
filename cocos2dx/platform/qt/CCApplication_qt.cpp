/**
* Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "CCApplication.h"
#include <unistd.h>
#include <sys/time.h>
#include <QTimer>
#include <QSystemInfo>

#include "CCDirector.h"

using namespace cocos2d;
QTM_USE_NAMESPACE

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication(int &argc, char **argv)
    : QApplication(argc, argv),
      m_timer(NULL),
      m_refCount(0)
{
	CC_ASSERT(! sm_pSharedApplication);
	sm_pSharedApplication = this;
    m_nAnimationInterval = 1.0f / 60.0f * 1000.0f;

#ifdef Q_OS_SYMBIAN
    QCoreApplication:setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);
#endif
}

void CCApplication::timerUpdate()
{
    // m_refCount is here to prevent calling the mainloop from nested event loops
    if (!m_refCount)
    {
        CCDirector::sharedDirector()->mainLoop();
    }
}

CCApplication::~CCApplication()
{
	CC_ASSERT(this == sm_pSharedApplication);
	sm_pSharedApplication = NULL;

    CC_SAFE_DELETE(m_timer);
}

void CCApplication::lock()
{
    m_refCount++;
}

void CCApplication::unlock()
{
    CC_ASSERT(m_refCount > 0);
    if (m_refCount > 0)
    {
        m_refCount--;
    }
}

int CCApplication::run()
{
    // Initialize instance and cocos2d.
    if (! initInstance() || ! applicationDidFinishLaunching())
    {
        return 0;
    }

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    m_timer->start(m_nAnimationInterval);
    return exec();
}

CCApplication::Orientation CCApplication::setOrientation(Orientation orientation)
{
	// swap width and height
	CCEGLView * pView = CCDirector::sharedDirector()->getOpenGLView();
	if (pView)
	{
		return (Orientation)pView->setDeviceOrientation(orientation);
	}
	return (Orientation)CCDirector::sharedDirector()->getDeviceOrientation();
}

void CCApplication::statusBarFrame(cocos2d::CCRect * rect)
{
	if (rect)
	{
        // No status bar
		*rect = CCRectMake(0, 0, 0, 0);
	}
}

void CCApplication::setAnimationInterval(double interval)
{
    m_nAnimationInterval = interval * 1000.0f;
    if (m_timer)
    {
        m_timer->start(m_nAnimationInterval);
    }
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
CCApplication& CCApplication::sharedApplication()
{
	CC_ASSERT(sm_pSharedApplication);
	return *sm_pSharedApplication;
}

ccLanguageType CCApplication::getCurrentLanguage()
{
    QSystemInfo sysInfo;
    QString lang = sysInfo.currentLanguage();

    if (lang == "zh")
    {
        return kLanguageChinese;
    }
    else if (lang == "fr")
    {
        return kLanguageFrench;
    }
    else if (lang == "it")
    {
        return kLanguageItalian;
    }
    else if (lang == "de")
    {
        return kLanguageGerman;
    }
    else if (lang == "es")
    {
        return kLanguageSpanish;
    }
    else if (lang == "ru")
    {
        return kLanguageRussian;
    }

    return kLanguageEnglish;
}
