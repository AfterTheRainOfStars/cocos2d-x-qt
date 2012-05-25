/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "JumpingGameScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

USING_NS_CC;

CCScene* JumpingGame::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
    JumpingGame *layer = JumpingGame::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool JumpingGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    // Generate a random seed
    srand(time(NULL));

    initBackground();
    initBlocks();
    initPlayer();
    initAudio();

    // Start background music
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music.ogg");

    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    CCScheduler::sharedScheduler()->scheduleUpdateForTarget(this, 0, false);

    return true;
}

void JumpingGame::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

bool JumpingGame::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    float winMidX = winSize.width * 0.5f;
    float winMidY = winSize.height * 0.5f;

    // Transform touch point to match the screen rotation.
    CCPoint touchLocation = CCDirector::sharedDirector()->convertToGL(touch->locationInView());

    if(touchLocation.y > winMidY)
    {
        // Jump if player is not already jumping.
        if(!m_playerJumping)
        {
            m_playerJumping = true;
            jump();
        }
    }
    else
    {
        if(touchLocation.x < winMidX)
            m_playerVelocity.x = -200.0f;
        else
            m_playerVelocity.x = 200.0f;
    }

    return true;
}

void JumpingGame::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    m_playerVelocity.x = 0.0f;
}

void JumpingGame::onExit()
{
    CCScheduler::sharedScheduler()->unscheduleUpdateForTarget(this);
}

void JumpingGame::update(ccTime dt)
{
    if(m_gameOver)
    {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->stopAllEffects();

        // Change the game state to GameOver
        CCScene *pScene = GameOver::scene();
        CCDirector::sharedDirector()->replaceScene(pScene);

        return;
    }

    updatePlayer();
    checkCollisions();
    scrollScene();
    updateBlocks();
    updateScore();
}

void JumpingGame::initBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Load the background image sprite and set the position to bottom left side of the screen.
    CCSprite* background = CCSprite::spriteWithFile("background.png");
    background->setAnchorPoint(ccp(0.0f, 0.0f));
    background->setPosition(ccp(0.0f, 0.0f));

    // Scale background to cover the whole window.
    CCRect bgRect = background->getTextureRect();
    background->setScaleX(winSize.width / bgRect.size.width);
    background->setScaleY(winSize.height / bgRect.size.height);

    // Add the background as a child to JumpingGame layer.
    this->addChild(background, 0);
}

void JumpingGame::initBlocks()
{
    // Load the block texture.
    CCTexture2D* blockTexture = CCTextureCache::sharedTextureCache()->addImage("block.png");

    // Create the first set of blocks.
    m_blocks[0] = CCSprite::spriteWithTexture(blockTexture);
    m_blocks[0]->setAnchorPoint(ccp(0.0f, 0.0f));
    m_blocks[0]->setPosition(ccp(30.0f, 100.0f));
    m_blocks[1] = CCSprite::spriteWithTexture(blockTexture);
    m_blocks[1]->setAnchorPoint(ccp(0.0f, 0.0f));
    m_blocks[1]->setPosition(ccp(200.0f, 290.0f));
    m_blocks[2] = CCSprite::spriteWithTexture(blockTexture);
    m_blocks[2]->setAnchorPoint(ccp(0.0f, 0.0f));
    m_blocks[2]->setPosition(ccp(80.0f, 520.0f));

    // Add the blocks as a child to JumpingGame layer.
    for(int i = 0; i < BLOCK_COUNT; i++)
    {
        this->addChild(m_blocks[i], 0);
    }
}

void JumpingGame::initPlayer()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Create player texture
    CCTexture2D *playerTexture = CCTextureCache::sharedTextureCache()->addImage("player.png");

    // Create player animation frames
    CCMutableArray<CCSpriteFrame*> *playerAnimationFrames = new CCMutableArray<CCSpriteFrame*>(7);
    for(int i = 0; i < 7; i++)
    {
        CCSpriteFrame *playerFrame = CCSpriteFrame::frameWithTexture(playerTexture, CCRectMake(128*i, 128*0, 128, 128));
        playerAnimationFrames->addObject(playerFrame);
    }

    // Create player animation from the animation frames
    CCAnimation* playerAnimation = CCAnimation::animationWithFrames(playerAnimationFrames);

    // Add player animation to animation cache
    CCAnimationCache::sharedAnimationCache()->addAnimation(playerAnimation, "PlayerAnimation");

    // Create player sprite from the first animation frame
    m_player = CCSprite::spriteWithSpriteFrame(playerAnimationFrames->getObjectAtIndex(0));
    m_player->setAnchorPoint(ccp(0.0f, 0.0f));
    CCRect playerRect = m_player->getTextureRect();
    m_player->setPosition(ccp((winSize.width - playerRect.size.width) / 2.0f, 0.0f));
    this->addChild(m_player);

    // Player animation frames are stored by the player animation
    playerAnimationFrames->release();

    m_playerVelocity = ccp(0.0f, 0.0f);
    m_playerScore = 0;
    m_playerJumping = false;
    m_gameOver = false;
}

void JumpingGame::initAudio()
{
    // Set background and audio effect volumes
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);

    // Preload background music
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music.ogg");

    // Preload audio effect
    SimpleAudioEngine::sharedEngine()->preloadEffect("jump.wav");
}

void JumpingGame::updatePlayer()
{
    float step = CCDirector::sharedDirector()->getAnimationInterval();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint playerPosition = m_player->getPosition();

    playerPosition.x += m_playerVelocity.x * step;

    if(m_playerJumping)
    {
        // Apply gravity and update player position in Y-axis
        const float gravity = -300.0f;
        m_playerVelocity.y += gravity * step;
        playerPosition.y += m_playerVelocity.y * step;
    }

    // Player dropped outside from the screen?
    if(playerPosition.y < -100)
    {
        m_gameOver = true;
    }

    // Prevent the player from moving outside from the screen
    CCRect playerRect = m_player->getTextureRect();
    if(playerPosition.x < 0.0f)
        playerPosition.x = 0.0f;
    else if (playerPosition.x > winSize.width - playerRect.size.width)
        playerPosition.x = winSize.width - playerRect.size.width;

    // Set the new player position
    m_player->setPosition(playerPosition);

    // Update player animation
    if(m_playerVelocity.y >= 300.0f)
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 0);
    else if(m_playerVelocity.y >= 150.0f)
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 1);
    else if(m_playerVelocity.y >= 50.0f)
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 2);
    else if(m_playerVelocity.y >= 0.0f)
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 3);
    else if(m_playerVelocity.y <= -40.0f)
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 4);
    else if(m_playerVelocity.y <= -100.0f)
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 5);
    else
        m_player->setDisplayFrameWithAnimationName("PlayerAnimation", 6);
}

void JumpingGame::checkCollisions()
{
    // Check player -> blocks collisions if player is moving down
    if(m_playerVelocity.y < 0.0f)
    {
        for(int i = 0; i < BLOCK_COUNT; i++)
        {
            CCRect playerRect = m_player->getTextureRect();
            playerRect.origin.x = m_player->getPositionX() + 30;
            playerRect.origin.y = m_player->getPositionY();
            playerRect.size.width -= 60;
            playerRect.size.height = 5;

            CCRect blockRect = m_blocks[i]->getTextureRect();
            blockRect.origin.x = m_blocks[i]->getPositionX() + 20;
            blockRect.origin.y = m_blocks[i]->getPositionY() + 15;
            blockRect.size.width -= 40;
            blockRect.size.height = 20;

            // Jump if player hits a block.
            if(CCRect::CCRectIntersectsRect(playerRect, blockRect))
                jump();
        }
    }
}

void JumpingGame::scrollScene()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCPoint playerPosition;
    playerPosition.x = m_player->getPositionX();
    playerPosition.y = m_player->getPositionY();

    float scrollBase = winSize.height * 0.30f;
    if(playerPosition.y > scrollBase)
    {
        float scrollMove = scrollBase - playerPosition.y;

        // Move blocks down
        for(int i = 0; i < BLOCK_COUNT; i++)
        {
            CCPoint blockPosition = m_blocks[i]->getPosition();
            blockPosition.y += scrollMove;
            m_blocks[i]->setPosition(blockPosition);
        }

        // Move player down
        playerPosition.y += scrollMove;
        m_player->setPosition(playerPosition);
    }
}

void JumpingGame::updateBlocks()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    for(int i = 0; i < BLOCK_COUNT; i++)
    {
        CCPoint blockPosition = m_blocks[i]->getPosition();
        CCRect blockRect = m_blocks[i]->getTextureRect();

        // Has block gone outside the screen?
        if(blockPosition.y < -blockRect.size.height)
        {
            // Move block on top of the screen
            blockPosition.x = rand()%(int)(winSize.width - blockRect.size.width + 60) - 30;
            blockPosition.y = winSize.height;

            m_blocks[i]->setPosition(blockPosition);
        }
    }
}

void JumpingGame::updateScore()
{
    if(m_playerVelocity.y > 0.0f)
    {
        m_playerScore++;

        CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
        const int scoreFlag = 12345;

        // Remove the old score label from the JumpingGame layer if there is one.
        removeChildByTag(scoreFlag, true);

        char scoreText[32];
        sprintf(scoreText, "SCORE: %d", m_playerScore);

        // Create score label and set the position on top left side of the screen
        CCLabelTTF* scoreLabel = CCLabelTTF::labelWithString(scoreText, "arial", 32);
        scoreLabel->setAnchorPoint(ccp(0.0f, 0.0f));
        scoreLabel->setPosition(ccp(0.0f, windowSize.height - scoreLabel->getTextureRect().size.height));

        // Add the new score label as a child to JumpingGame layer.
        this->addChild(scoreLabel, 0, scoreFlag);
    }
}

void JumpingGame::jump()
{
    if(m_playerJumping)
    {
        // Play jump audio effect
        SimpleAudioEngine::sharedEngine()->playEffect("jump.wav", false);

        m_playerVelocity.y = 500.0f;
    }
}

