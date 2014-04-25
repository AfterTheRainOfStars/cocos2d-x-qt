/**
* Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "AppDelegate.h"

int main(int argc, char** argv)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_QT)
    AppDelegate app(argc, argv);
#else
    AppDelegate app;
#endif
    return cocos2d::CCApplication::sharedApplication().run();
}
