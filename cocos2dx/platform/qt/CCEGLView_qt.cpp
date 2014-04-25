/**
* Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "CCEGLView_qt.h"

#include "CCGL.h"

#include "CCSet.h"
#include "ccMacros.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCIMEDispatcher.h"
#include "CCApplication.h"

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif

#include <QEvent>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <aknappui.h>
#endif

#ifdef MEEGO_EDITION_HARMATTAN
#include <qmeegographicssystemhelper.h>
#endif

PFNGLGENERATEMIPMAPOESPROC glGenerateMipmapOES = NULL;
PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffersOES = NULL;
PFNGLBINDFRAMEBUFFEROESPROC glBindFramebufferOES = NULL;
PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2DOES = NULL;
PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffersOES = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSOESPROC glCheckFramebufferStatusOES = NULL;

#define LOAD_EXTENSION(type, name) (name = (type)eglGetProcAddress(#name))
#define NULL_EXTENSION(name) (name = name##_null)

void GL_APIENTRY glBindFramebufferOES_null(GLenum target, GLuint framebuffer)
{
    Q_UNUSED(target);
    Q_UNUSED(framebuffer);
    //CCLOGERROR("%s\n", "glBindFramebufferOES not supported");
}

void GL_APIENTRY glDeleteFramebuffersOES_null(GLsizei n,
                                              const GLuint* framebuffers)
{
    Q_UNUSED(n);
    Q_UNUSED(framebuffers);
    //CCLOGERROR("%s\n", "glDeleteFramebuffersOES not supported");
}

void GL_APIENTRY glGenFramebuffersOES_null(GLsizei n, GLuint* framebuffers)
{
    Q_UNUSED(n);
    Q_UNUSED(framebuffers);
    //CCLOGERROR("%s\n", "glGenFramebuffersOES not supported");
}

GLenum GL_APIENTRY glCheckFramebufferStatusOES_null(GLenum target)
{
    Q_UNUSED(target);
    //CCLOGERROR("%s\n", "glCheckFramebufferStatusOES not supported");
    return 0;
}

void GL_APIENTRY glFramebufferTexture2DOES_null(GLenum target,
                                                GLenum attachment,
                                                GLenum textarget,
                                                GLuint texture,
                                                GLint level)
{
    Q_UNUSED(target);
    Q_UNUSED(attachment);
    Q_UNUSED(textarget);
    Q_UNUSED(texture);
    Q_UNUSED(level);
    //CCLOGERROR("%s\n", "glFramebufferTexture2DOES not supported");
}

void GL_APIENTRY glGenerateMipmapOES_null(GLenum target)
{
    Q_UNUSED(target);
    //CCLOGERROR("%s\n", "glGenerateMipmapOES not supported");
}

bool initExtensions()
{
    const char* extensions = (const char*)glGetString(GL_EXTENSIONS);

    CCLOG(extensions);

    if(!LOAD_EXTENSION(PFNGLGENERATEMIPMAPOESPROC, glGenerateMipmapOES))
        NULL_EXTENSION(glGenerateMipmapOES);

    if(!LOAD_EXTENSION(PFNGLGENFRAMEBUFFERSOESPROC, glGenFramebuffersOES))
        NULL_EXTENSION(glGenFramebuffersOES);

    if(!LOAD_EXTENSION(PFNGLBINDFRAMEBUFFEROESPROC, glBindFramebufferOES))
        NULL_EXTENSION(glBindFramebufferOES);

    if(!LOAD_EXTENSION(PFNGLFRAMEBUFFERTEXTURE2DOESPROC, glFramebufferTexture2DOES))
        NULL_EXTENSION(glFramebufferTexture2DOES);

    if(!LOAD_EXTENSION(PFNGLDELETEFRAMEBUFFERSOESPROC, glDeleteFramebuffersOES))
        NULL_EXTENSION(glDeleteFramebuffersOES);

    if(!LOAD_EXTENSION(PFNGLCHECKFRAMEBUFFERSTATUSOESPROC, glCheckFramebufferStatusOES))
        NULL_EXTENSION(glCheckFramebufferStatusOES);

    return true;
}

NS_CC_BEGIN;

#ifdef MEEGO_EDITION_HARMATTAN

namespace Maliit {
    enum OrientationAngle {
        Angle0 = 0,
        Angle90 = 90,
        Angle180 = 180,
        Angle270 = 270
    };
}

#endif


static CCEGLView* s_pMainWindow = NULL;

CCEGLView::CCEGLView(QWidget* parent):
    QWidget(parent),
    m_bCaptured(false),
    m_bOrientationReverted(false),
    m_bOrientationInitVertical(false),
    m_pDelegate(NULL),
    bIsInit(false),
    m_eInitOrientation(CCDeviceOrientationPortrait),
    m_eOrientation(CCDeviceOrientationPortrait),
    m_fScreenScaleFactor(1.0f),
    m_eglDisplay(NULL),
    m_eglConfig(NULL),
    m_eglSurface(NULL),
    m_eglContext(NULL)
{
    m_sSizeInPoint.width = m_sSizeInPoint.height = 0;
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_StyledBackground, false);
    setAttribute(Qt::WA_PaintUnclipped);

    setAttribute(Qt::WA_InputMethodEnabled, true);
    setAttribute(Qt::WA_KeyboardFocusChange, true);
    setInputMethodHints(Qt::ImhNoPredictiveText);

    qApp->installEventFilter(this);

#ifdef MS_USE_TOUCH_EVENTS
    setAttribute(Qt::WA_AcceptTouchEvents);

    for(int i = 0; i < MAX_TOUCH_POINTS; i++)
        m_pTouch[i] = NULL;
#else
    m_pTouch = NULL;
#endif

#ifdef MEEGO_EDITION_HARMATTAN
    QMeeGoGraphicsSystemHelper::setSwitchPolicy(QMeeGoGraphicsSystemHelper::NoSwitch);

    QInputContext *ic = QInputContextFactory::create("MInputContext", 0);
    if (ic)
    {
        qApp->setInputContext(ic);
    }
#endif // MEEGO_EDITION_HARMATTAN

#if defined(Q_OS_SYMBIAN)
    CAknAppUiBase::TAppUiOrientation uiOrientation;

    switch(m_eInitOrientation)
    {
    case kCCDeviceOrientationPortrait:
    case kCCDeviceOrientationPortraitUpsideDown:
        uiOrientation = CAknAppUi::EAppUiOrientationPortrait;
        m_eOrientation = kCCDeviceOrientationPortrait;
        break;

    case kCCDeviceOrientationLandscapeLeft:
    case kCCDeviceOrientationLandscapeRight:
        uiOrientation = CAknAppUi::EAppUiOrientationLandscape;
        m_eOrientation = kCCDeviceOrientationLandscapeLeft;
        break;

    default:
        uiOrientation = CAknAppUi::EAppUiOrientationPortrait;
        m_eOrientation = kCCDeviceOrientationPortrait;
    }

    CAknAppUi *ui = dynamic_cast<CAknAppUi *>(CEikonEnv::Static()->AppUi());
    TRAPD(error,
        if(ui) {
            ui->SetOrientationL(uiOrientation);
        }
    );

    Q_UNUSED(error);
#elif defined(MEEGO_EDITION_HARMATTAN)
    // TODO: The code below doesn't work on harmattan, FIX! (if possible and needed)
    /*
    switch(m_eInitOrientation)
    {
    case kCCDeviceOrientationPortrait:
    case kCCDeviceOrientationPortraitUpsideDown:
        //setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        setAttribute(Qt::WA_LockPortraitOrientation, true);
        break;

    case kCCDeviceOrientationLandscapeLeft:
    case kCCDeviceOrientationLandscapeRight:
        //setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
        setAttribute(Qt::WA_LockLandscapeOrientation, true);
        break;

    default:
        //setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        setAttribute(Qt::WA_LockPortraitOrientation, true);
        break;
    }
    */
#endif
}

CCEGLView::~CCEGLView()
{
}

bool CCEGLView::Create()
{
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#endif

    CCLOG("Create: width = %d height = %d", width(), height());

    s_pMainWindow = this;

    m_rcViewPort.size.width = width();
    m_rcViewPort.size.height = height();

#ifdef MEEGO_EDITION_HARMATTAN
    showFullScreen();
#endif

    if (!initEGL())
    {
        return false;
    }

    activateWindow();
    setFocus();
    grabKeyboard();

    return true;
}

CCSize CCEGLView::getSize()
{
    CCLOG("getSize() width = %.1f height = %.1f",
          m_rcViewPort.size.width, m_rcViewPort.size.height);

    return m_rcViewPort.size;
}

float CCEGLView::getMainScreenScale()
{
    // TODO: return correct value
    return -1.0f;
}

bool CCEGLView::isOpenGLReady()
{
    return bIsInit;
}

void CCEGLView::release()
{
    releaseEGL();

    qApp->exit(0);
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate * pDelegate)
{
    m_pDelegate = pDelegate;
}

void CCEGLView::swapBuffers()
{
    if(bIsInit)
    {
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
    }
}

int CCEGLView::setDeviceOrientation(int eOrientation)
{
#if defined(Q_OS_SYMBIAN)
    CAknAppUiBase::TAppUiOrientation uiOrientation;

    switch(eOrientation)
    {
    case kCCDeviceOrientationPortrait:
    case kCCDeviceOrientationPortraitUpsideDown:
        uiOrientation = CAknAppUi::EAppUiOrientationPortrait;
        m_eOrientation = kCCDeviceOrientationPortrait;
        break;

    case kCCDeviceOrientationLandscapeLeft:
    case kCCDeviceOrientationLandscapeRight:
        uiOrientation = CAknAppUi::EAppUiOrientationLandscape;
        m_eOrientation = kCCDeviceOrientationLandscapeLeft;
        break;

    default:
        uiOrientation = CAknAppUi::EAppUiOrientationPortrait;
        m_eOrientation = kCCDeviceOrientationPortrait;
    }

    CAknAppUi *ui = dynamic_cast<CAknAppUi *>(CEikonEnv::Static()->AppUi());
    TRAPD(error,
        if(ui) {
            ui->SetOrientationL(uiOrientation);
        }
    );

    Q_UNUSED(error);

    m_rcViewPort.size.width = width();
    m_rcViewPort.size.height = height();

    // Return portrait orientation to prevent cocos from rotating the scene
    return kCCDeviceOrientationPortrait;
#elif defined(MEEGO_EDITION_HARMATTAN)
    /* Tell cocos2d to rotate 90-degrees on ccw because
     * the device is on landscape mode instead of portrait mode.
     */
    switch(eOrientation)
    {
    case kCCDeviceOrientationPortrait:
        m_eOrientation = kCCDeviceOrientationLandscapeRight;
        break;

    case kCCDeviceOrientationPortraitUpsideDown:
        m_eOrientation = kCCDeviceOrientationLandscapeLeft;
        break;

    case kCCDeviceOrientationLandscapeLeft:
        m_eOrientation = kCCDeviceOrientationPortrait;
        break;

    case kCCDeviceOrientationLandscapeRight:
        m_eOrientation = kCCDeviceOrientationPortraitUpsideDown;
        break;

    default:
        m_eOrientation = kCCDeviceOrientationPortrait;
        break;
    }
    updateVkbOrientation();
    return m_eOrientation;
#endif
}

int CCEGLView::getDeviceOrientation() const
{
    return m_eOrientation;
}

void CCEGLView::setViewPortInPoints(float x, float y, float w, float h)
{
    CCLOG("setViewPortInPoints x = %.1f y = %.1f w = %.1f h = %.1f", x, y, w, h);

    if (bIsInit) {
        float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
        CCLOG("factor = %d", factor);

        glViewport((GLint)(x * factor) + m_rcViewPort.origin.x,
                (GLint)(y * factor) + m_rcViewPort.origin.y,
                (GLint)(w * factor),
                (GLint)(h * factor));
    }
}

void CCEGLView::setScissorInPoints(float x, float y, float w, float h)
{
    CCLOG("setScissorInPointsx = %d y = %d w = %d h = %d", x, y, w, h);

    if (bIsInit) {
        float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
        glScissor((GLint)(x * factor) + m_rcViewPort.origin.x,
                (GLint)(y * factor) + m_rcViewPort.origin.y,
                (GLint)(w * factor),
                (GLint)(h * factor));
    }
}

void CCEGLView::updateVkbOrientation()
{
#ifdef MEEGO_EDITION_HARMATTAN
    Maliit::OrientationAngle mtfOrient;

    switch (m_eOrientation)
    {
    case kCCDeviceOrientationPortrait:
        mtfOrient = Maliit::Angle0;
        break;
    case kCCDeviceOrientationLandscapeLeft:
        mtfOrient = Maliit::Angle90;
        break;
    case kCCDeviceOrientationPortraitUpsideDown:
        mtfOrient = Maliit::Angle180;
        break;
    default: // kCCDeviceOrientationLandscapeRight
        mtfOrient = Maliit::Angle270;
        break;
    }

    QMetaObject::invokeMethod(qApp->inputContext(),
        "notifyOrientationChanged",
        Q_ARG(Maliit::OrientationAngle, mtfOrient));
#endif
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    QInputContext *ic = qApp->inputContext();

    if(!ic)
        return;

    if(bOpen)
    {
        QWidget* focusWidget = qApp->focusWidget();
        if(focusWidget)
        {
            focusWidget->setInputMethodHints(Qt::ImhNoPredictiveText);
            focusWidget->setAttribute(Qt::WA_InputMethodEnabled, true);
            ic->setFocusWidget(focusWidget);

            QEvent request(QEvent::RequestSoftwareInputPanel);
            ic->filterEvent(&request);
            updateVkbOrientation();
        }
    }
    else
    {
        QEvent close(QEvent::CloseSoftwareInputPanel);
        ic->filterEvent(&close);
        ic->reset();
    }
}

bool CCEGLView::canSetContentScaleFactor()
{
    // TODO: Implement
    return false;
}

void CCEGLView::setContentScaleFactor(float contentScaleFactor)
{
    CCLog("could not set contentScaleFactor after initialized");
    // TODO: Implement
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
    CC_ASSERT(s_pMainWindow);
    return *s_pMainWindow;
}

void CCEGLView::inputMethodEvent(QInputMethodEvent *event)
{
#if defined(Q_OS_SYMBIAN)
    // This section of code is used when the virtual keyboard is in "keypad"-mode
    static bool charStart = true;
    static char lastChar = 0;

    QString commitString = event->commitString();
    if(commitString.length() > 0)
    {
        charStart = true;
        lastChar = 0;
    }
    else
    {
        QString preeditString = event->preeditString();
        if(preeditString.length() > 0)
        {
            char newChar = preeditString.at(preeditString.length()-1).toAscii();
            if(newChar != lastChar)
            {
                if(!charStart)
                    CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();

                CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(&newChar, 1);
                lastChar = newChar;
            }

            charStart = false;
        }
    }
#elif defined(MEEGO_EDITION_HARMATTAN)
    QString commitString = event->commitString();
    if(commitString.length() > 0)
    {
        char newChar = commitString.at(commitString.length()-1).toAscii();
        CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(&newChar, 1);
    }
#endif
}

bool CCEGLView::eventFilter(QObject *object, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::ActivationChange:
        {
            if(isActiveWindow())
            {
                CCLOG("Activate");
                CCApplication::sharedApplication().applicationWillEnterForeground();
            }
            else
            {
                CCLOG("Deactivate");
                CCApplication::sharedApplication().applicationDidEnterBackground();
            }

            return false;
        }break;

    default:
        {
        }break;
    }

    return QObject::eventFilter(object, event);
}

void CCEGLView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void CCEGLView::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width();
    int h = event->size().height();

    if(event->size().width() == event->oldSize().width() &&
       event->size().height() == event->oldSize().width()) {
        return;
    }

    CCLOG("Resize viewport to %d x %d", w, h);
    glViewport(0, 0, w, h);

    QWidget::resizeEvent(event);
}

#ifdef MS_USE_TOUCH_EVENTS

bool CCEGLView::handleTouchEvent(QEvent* event)
{
    CCSet beganSet;
    CCSet updateSet;
    CCSet releaseSet;

    QList<QTouchEvent::TouchPoint> touchPoints =
            static_cast<QTouchEvent *>(event)->touchPoints();
    foreach(const QTouchEvent::TouchPoint &tp, touchPoints)
    {
        int ind = tp.id();
        if(ind >= MAX_TOUCH_POINTS)
            continue;

        if(m_pTouch[ind])
        {
            if(tp.state() & Qt::TouchPointReleased)
            {
                // release
                m_pTouch[ind]->SetTouchInfo(tp.pos().x(), tp.pos().y());
                releaseSet.addObject(m_pTouch[ind]);
                m_pTouch[ind]->release();
                m_pTouch[ind] = NULL;
            }
            else
            {
                // update
                m_pTouch[ind]->SetTouchInfo(tp.pos().x(), tp.pos().y());
                updateSet.addObject(m_pTouch[ind]);
            }
        }
        else
        {
            // begin
            m_pTouch[ind] = new CCTouch;
            m_pTouch[ind]->SetTouchInfo(tp.pos().x(), tp.pos().y());
            beganSet.addObject(m_pTouch[ind]);
        }
    }

    if(beganSet.count() > 0)
        m_pDelegate->touchesBegan(&beganSet, NULL);
    if(updateSet.count() > 0)
        m_pDelegate->touchesMoved(&updateSet, NULL);
    if(releaseSet.count() > 0)
        m_pDelegate->touchesEnded(&releaseSet, NULL);

    return true;
}

bool CCEGLView::handleKeyEvent(QEvent* event)
{
    QKeyEvent* ke = (QKeyEvent*)event;

    switch(ke->key())
    {
    case Qt::Key_Backspace:
        CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
        return true;

    case Qt::Key_Tab:
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        CCIMEDispatcher::sharedDispatcher()->dispatchInsertText("\n", 1);
        return true;

    default:
        {
            QString str = ke->text();

            QByteArray ba = str.toAscii();
            if(ba.size() > 0)
            {
                const char* c = ba.constData();
                CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(c, 1);
            }

            return true;
        }
    }

    return false;
}

bool CCEGLView::event(QEvent* event)
{
    switch(event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
        if (handleTouchEvent(event))
            return true;
        else
            break;

    case QEvent::KeyPress:
        if (handleKeyEvent(event))
            return true;
        else
            break;

    case QEvent::KeyRelease:
        return true;

    default:
        break;
    }

    return QWidget::event(event);
}
#else
void CCEGLView::mousePressEvent(QMouseEvent *event)
{
    QMouseEvent* mouseEvent = (QMouseEvent*)event;
    CCPoint point(mouseEvent->x(), mouseEvent->y());

    if(m_pTouch)
        m_pTouch->release();

    m_pTouch = new CCTouch;
    m_pTouch->SetTouchInfo(point.x, point.y);

    CCSet set;
    set.addObject(m_pTouch);

    m_pDelegate->touchesBegan(&set, NULL);

    QWidget::mousePressEvent(event);
}

void CCEGLView::mouseReleaseEvent(QMouseEvent *event)
{
    QMouseEvent* mouseEvent = (QMouseEvent*)event;
    CCPoint point(mouseEvent->x(), mouseEvent->y());

    if(!m_pTouch)
        m_pTouch = new CCTouch;

    m_pTouch->SetTouchInfo(point.x, point.y);

    CCSet set;
    set.addObject(m_pTouch);

    m_pTouch->release();
    m_pTouch = NULL;

    m_pDelegate->touchesEnded(&set, NULL);

    QWidget::mouseReleaseEvent(event);
}

void CCEGLView::mouseMoveEvent(QMouseEvent *event)
{
    QMouseEvent* mouseEvent = (QMouseEvent*)event;
    CCPoint point(mouseEvent->x(), mouseEvent->y());

    if(!m_pTouch)
        m_pTouch = new CCTouch;

    m_pTouch->SetTouchInfo(point.x, point.y);

    CCSet set;
    set.addObject(m_pTouch);

    m_pDelegate->touchesMoved(&set, NULL);

    QWidget::mouseMoveEvent(event);
}
#endif

bool CCEGLView::initEGL()
{
    if(bIsInit)
        return true;

    EGLNativeWindowType nativeWindow = (EGLNativeWindowType)this->winId();
    EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;

#ifdef Q_OS_LINUX
    nativeDisplay = (EGLNativeDisplayType)x11Info().display();
#endif // Q_OS_LINUX

#ifdef Q_OS_SYMBIAN
    nativeWindow = (EGLNativeWindowType)(winId()->DrawableWindow());
    nativeDisplay = EGL_DEFAULT_DISPLAY;
#endif // Q_OS_SYMBIAN

    m_eglDisplay = eglGetDisplay(nativeDisplay);
    if(!m_eglDisplay)
    {
        return false;
    }

    if(!eglInitialize(m_eglDisplay, NULL, NULL))
    {
        return false;
    }

    eglBindAPI(EGL_OPENGL_ES_API);

    int configAttributes[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_DEPTH_SIZE, 16,
        EGL_NONE,
    };

    int configCount = 0;
    if(!eglChooseConfig(m_eglDisplay, configAttributes, &m_eglConfig, 1,
                        &configCount) || configCount != 1)
    {
        CCLOG("eglChooseConfig failed");
        return false;
    }

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig,
                                          nativeWindow, NULL);
    if(!m_eglSurface)
    {
        CCLOG("eglCreateWindowSurface failed");
        return false;
    }

    int contextAttributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 1, // 2
        EGL_NONE,
    };

    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL,
                                    contextAttributes);
    if(m_eglContext == EGL_NO_CONTEXT)
    {
        CCLOG("eglCreateContext failed");
        return false;
    }

    if(!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
    {
        CCLOG("eglMakeCurrent failed");
        return false;
    }

    if(!initExtensions())
    {
        CCLOG("initExtensions failed");
        return false;
    }

    bIsInit = true;

    return true;
}

void CCEGLView::releaseEGL()
{
    if(m_eglContext)
    {
        eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                       EGL_NO_CONTEXT);
        eglDestroyContext(m_eglDisplay, m_eglContext);
        m_eglContext = NULL;
    }

    if(m_eglSurface)
    {
        eglDestroySurface(m_eglDisplay, m_eglSurface);
        m_eglSurface = NULL;
    }

    m_eglConfig = NULL;

    if(m_eglDisplay)
    {
        eglTerminate(m_eglDisplay);
        m_eglDisplay = NULL;
    }

    bIsInit = false;
}

NS_CC_END;
