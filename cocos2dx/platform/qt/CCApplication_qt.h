/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef CCAPPLICATION_QT_H_
#define CCAPPLICATION_QT_H_

#include <QApplication>
#include <Qt/qobject.h>
#include <QTimer>

#include "CCCommon.h"
#include "CCGeometry.h"

using namespace cocos2d;

NS_CC_BEGIN;

class CC_DLL CCApplication : public QApplication {
    Q_OBJECT
public:
    CCApplication(int &argc, char **argv);
    virtual ~CCApplication();

	/**
	 @brief	Implement for initialize OpenGL instance, set source path, etc...
	 */
	virtual bool initInstance() = 0;

	/**
	 @brief	Implement CCDirector and CCScene init code here.
	 @return true    Initialize success, app continue.
	 @return false   Initialize failed, app terminate.
	 */
	virtual bool applicationDidFinishLaunching() = 0;

	/**
	 @brief  The function be called when the application enter background
	 @param  the pointer of the application
	 */
	virtual void applicationDidEnterBackground() = 0;

	/**
	 @brief  The function be called when the application enter foreground
	 @param  the pointer of the application
	 */
	virtual void applicationWillEnterForeground() = 0;

	/**
	 @brief	Callback by CCDirector for limit FPS.
	 @interval       The time, which expressed in second in second, between current frame and next.
	 */
	void setAnimationInterval(double interval);

	typedef enum
	{
		/// Device oriented vertically, home button on the bottom
		kOrientationPortrait = 0,
		/// Device oriented vertically, home button on the top
		kOrientationPortraitUpsideDown = 1,
		/// Device oriented horizontally, home button on the right
		kOrientationLandscapeLeft = 2,
		/// Device oriented horizontally, home button on the left
		kOrientationLandscapeRight = 3,
	}Orientation;

	/**
	 @brief	Callback by CCDirector for change device orientation.
	 @orientation    The defination of orientation which CCDirector want change to.
	 @return         The actual orientation of the application.
	 */
	Orientation setOrientation(Orientation orientation);

	/**
	 @brief	Get status bar rectangle in EGLView window.
	 */
    void statusBarFrame(cocos2d::CCRect * rect);

	/**
	 @brief	Run the message loop.
	 */
	int run();

	/**
	 @brief	Get current applicaiton instance.
	 @return Current application instance pointer.
	 */
	static CCApplication& sharedApplication();

	/**
	 @brief Get current language config
	 @return Current language config
	 */
	static ccLanguageType getCurrentLanguage();

    void lock();
    void unlock();

public slots:
    void timerUpdate();

public:
    QTimer *m_timer;
    int m_refCount;

protected:
    long m_nAnimationInterval;
	static CCApplication * sm_pSharedApplication;
};

NS_CC_END;

#endif /* CCAPPLICATION_QT_H_ */
