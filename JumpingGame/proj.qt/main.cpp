/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "AppDelegate.h"

#include "CCFileUtils.h"

int main(int argc, char **argv)
{
#ifdef Q_OS_SYMBIAN
        CCFileUtils::sharedFileUtils()->setResourcePath("/private/E8AA3FAF/resources");
#else
        CCFileUtils::sharedFileUtils()->setResourcePath("/opt/jumpinggame/resources");
#endif

    // create the application instance
    AppDelegate app(argc, argv);
    app.setOrientation(CCApplication::kOrientationPortrait);
    return app.run();
}

