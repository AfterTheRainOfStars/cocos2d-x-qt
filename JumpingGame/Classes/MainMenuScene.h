#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(MainMenu);

    virtual void menuPlayCallback(CCObject* pSender);
    virtual void menuExitCallback(CCObject* pSender);

    void initMenu();
    void initBackground();
private:
};

#endif // __MAINMENU_SCENE_H__
