Cocos2D-x Qt
============

This preview version is based on cocos2d-1.0.1-x-0.13.0-beta release. Most of
the functionality exists, but is likely to contain lots of bugs and ugly hacks.

Building Cocos2D-x
==================

- Download and install Qt SDK 1.2.1 (or newer) from http://qt.nokia.com/downloads
- Start Qt Creator
- Open cocos2d/cocos2d-qt.pro
- Build (deployment is not required)

After this you can build the example applications.


Building and running the Cocos2D-x Helloworld
---------------------------------------------

- Open cocos2d/HelloWorld/qt/helloworld.pro
- Build, deploy and run the app


Building and running the Cocos2D-x HelloLua
-------------------------------------------

- Open cocos2d/HelloLua/qt/hellolua-qt.pro
- Build, deploy and run the app


Building and running the Cocos2D-x tests
----------------------------------------

- Open cocos2d/tests/test.qt/test.pro
- Build, deploy and run the app


Other build options
-------------------

See cocos2d-qt-common.pri.


Tested to work with
===================

- QtSDK 1.2.1
- C7 (Anna), N8 (Belle), 808 (Belle FP1), N9 (MeeGo 1.2 Harmattan, PR1.2)


Status of current implementation
================================

- Box2D: ok
- Chipmunk: ok
- CocosDenshion: ok
  * Uses Qt GameEnabler for audio playback
    (http://projects.developer.nokia.com/qtgameenabler)
  * Supports only ogg vorbis and wav files
  * The playback latency is rather high (>150ms)
- Cocos2d-x
  * CCAccelerometer: ok (uses QAccelerometer)
  * CCApplication: ok (the only difference with other platforms in the API is
    the CCApplication's constructor, which now needs the command line arguments.
    This may change in near future.)
  * CCEGLView: needs some cleanup, it is a bit messy
  * CCFileUtils: ok (uses QFile)
  * CCImage: ok (uses QImage for loading the images and QPainter for drawing
    text)
  * CCSaxParser: ok (uses Qt's sax parser instead of libxml)
  * CCUserDefault: uses QSettings instead of libxml, but the implementation
    is #ifdeffed into CCUserDefault.cpp. The qt specific implementation
    needs to be moved into a separate file, CCUserDefault_qt.cpp)
- Lua: ok
- Project templates
  * Missing, use the HelloWorld example as a base for your own project
- HelloWorld, HelloLua, Tests: ok (but some of the tests do not scale nicely
  onto the display)


Known issues & tips
===================


Effects -tests do not work properly on Symbian
----------------------------------------------

The tests require GL_OES_framebuffer_object extension, which is not available
on Symbian devices.


Deployment of the tests fails with N9 and Qt Creator 2.4.x
----------------------------------------------------------

Reason;
Qt Creator tries to upload the package into /tmp, which is only a
4Mb tmpfs.

See also;
https://bugreports.qt-project.org/browse/QTCREATORBUG-6859

Workaround for QtSdk running on Linux;

 $ cd ~/QtSDK/QtCreator/lib/qtcreator/plugins/Nokia
 $ gedit hack.python

Add the following to the file.

#!/usr/bin/env python

with open("libRemoteLinux.so", "rb") as f:
     bytes = f.read()
bytes = bytes.replace("/tmp", "/tm2")
with open("libRemoteLinux.so","wb") as f:
     f.write(bytes)

This script replaces /tmp with /tm2 from libRemoteLinux.so. You might want
to backup libRemoteLinux.so before running the script.

 $ chmod +x hack.python
 $ ./hack.python

Create /tm2 folder into your N9 (see the login information from
SDK Connectivity tools' Connectivity Details)

 $ ssh developer@192.168.2.15
 $ devel-su
 $ mkdir /tm2
 $ chmod 755 /tm2

And now the deployment should work ok.


Debugging does not work with QtSDK 1.2.1 and Belle FP1
------------------------------------------------------

- http://www.developer.nokia.com/Community/Discussion/showthread.php?234872-Cann
ot-debug-deploy-on-symbian-belle-after-upgrade-to-fp1-.1404
