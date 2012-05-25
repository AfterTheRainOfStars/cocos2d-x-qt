/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef __JUMPINGGAME_SCENE_H__
#define __JUMPINGGAME_SCENE_H__

#include "cocos2d.h"

#define BLOCK_COUNT 3

class JumpingGame : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(JumpingGame);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    virtual void onExit();
    virtual void update(ccTime dt);

    void initBackground();
    void initBlocks();
    void initPlayer();
    void initAudio();

    void updatePlayer();
    void checkCollisions();
    void scrollScene();
    void updateBlocks();
    void updateScore();

    void jump();
private:
    CCSprite* m_player;
    CCPoint m_playerVelocity;
    int m_playerScore;
    bool m_playerJumping;
    bool m_gameOver;

    CCSprite* m_blocks[BLOCK_COUNT];
};

#endif // __JUMPINGGAME_SCENE_H__
