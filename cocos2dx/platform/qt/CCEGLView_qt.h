/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef CCEGLVIEWQT_H_
#define CCEGLVIEWQT_H_

#include "CCCommon.h"
#include "CCGeometry.h"

#include <QtGui>
#include <QWidget>

#include <EGL/egl.h>

/*
#ifdef Q_OS_SYMBIAN
#define MS_USE_TOUCH_EVENTS
#define MAX_TOUCH_POINTS 5
#endif
*/

//#ifdef Q_WS_MAEMO_6
#define MS_USE_TOUCH_EVENTS
#define MAX_TOUCH_POINTS 5
//#endif

bool initExtensions();

NS_CC_BEGIN;

class CCSet;
class CCTouch;
class EGLTouchDelegate;

class CC_DLL CCEGLView: public QWidget
{
    Q_OBJECT
public:
    CCEGLView(QWidget* parent = NULL);
    virtual ~CCEGLView();

    virtual bool Create();

    CCSize getSize();
    bool isOpenGLReady();
    void release();
    void setTouchDelegate(EGLTouchDelegate * pDelegate);
    void swapBuffers();
    bool canSetContentScaleFactor();
    void setContentScaleFactor(float contentScaleFactor);

    int setDeviceOrientation(int eOrientation);
    int getDeviceOrientation() const;
    void setViewPortInPoints(float x, float y, float w, float h);
    void setScissorInPoints(float x, float y, float w, float h);

    void setIMEKeyboardState(bool bOpen);

    float getMainScreenScale();

    bool isIpad() { return false; }

    /**
     @brief	get the shared main open gl window
     */
    static CC_DLL CCEGLView& sharedOpenGLView();

protected: // From QWidget (and other base classes)
    void inputMethodEvent(QInputMethodEvent *event);
    bool eventFilter(QObject *object, QEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    bool initEGL();
    void releaseEGL();
    bool handleKeyEvent(QEvent*);
    void updateVkbOrientation();
#ifdef MS_USE_TOUCH_EVENTS
    bool handleTouchEvent(QEvent*);
    bool event(QEvent*);
#else
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
#endif

private:
    bool m_bCaptured;
    bool m_bOrientationReverted;
    bool m_bOrientationInitVertical;

#ifdef MS_USE_TOUCH_EVENTS
    CCTouch *m_pTouch[MAX_TOUCH_POINTS];
#else
    CCTouch *m_pTouch;
#endif
    EGLTouchDelegate * m_pDelegate;

    CCSize m_sSizeInPixel;
    CCSize m_sSizeInPoint;
    CCRect m_rcViewPort;

    bool bIsInit;
    int m_eInitOrientation;
    int m_eOrientation;
    float m_fScreenScaleFactor;

    EGLDisplay  m_eglDisplay;
    EGLConfig   m_eglConfig;
    EGLSurface  m_eglSurface;
    EGLContext  m_eglContext;
};

NS_CC_END;

using namespace cocos2d;

#endif /* CCEGLVIEWLINUX_H_ */
