/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CCEGLView_qt.h"

using namespace cocos2d;
QTM_USE_NAMESPACE

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication(int &argc, char **argv):
    QApplication(argc, argv),
    m_timer(NULL)
{
	CC_ASSERT(! sm_pSharedApplication);
	sm_pSharedApplication = this;
    m_nAnimationInterval = 1.0f / 60.0f * 1000.0f;

#ifdef Q_OS_SYMBIAN
    QCoreApplication:setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);
#endif
}

CCApplication::~CCApplication()
{
	CC_ASSERT(this == sm_pSharedApplication);
	sm_pSharedApplication = NULL;

    CC_SAFE_DELETE(m_timer);
}

void CCApplication::setAnimationInterval(double interval)
{
    qDebug() << "setAnimationInterval";

    m_nAnimationInterval = interval * 1000.0f;
    if (m_timer)
    {
        m_timer->start(m_nAnimationInterval);
    }
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

int CCApplication::run()
{
    // Initialize instance and cocos2d.
    //if (! initInstance() || ! applicationDidFinishLaunching())
    if (!applicationDidFinishLaunching())
    {
        return 0;
    }

    CCEGLView& view = CCEGLView::sharedOpenGLView();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    m_timer->start(m_nAnimationInterval);

    /*
    if (!m_timer)
    {
        setAnimationInterval((double)m_nAnimationInterval / 1000.0f);
    }
    */

    return exec();
}

CCApplication& CCApplication::sharedApplication()
{
    CC_ASSERT(sm_pSharedApplication);
    return *sm_pSharedApplication;
}

void CCApplication::timerUpdate()
{
    CCDirector::sharedDirector()->mainLoop();
}
