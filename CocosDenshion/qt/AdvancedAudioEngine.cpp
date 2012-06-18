/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "AdvancedAudioEngine.h"
#include "QtAdvancedAudioPlayer.h"

namespace CocosDenshion {

static AdvancedAudioPlayer* oAudioPlayer;

AdvancedAudioEngine::AdvancedAudioEngine()
{
    oAudioPlayer = QtAdvancedAudioPlayer::sharedPlayer();
}

AdvancedAudioEngine::~AdvancedAudioEngine()
{
}

AdvancedAudioEngine* AdvancedAudioEngine::sharedEngine()
{
    static AdvancedAudioEngine s_SharedEngine;
    return &s_SharedEngine;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void AdvancedAudioEngine::end()
{
    // TODO: ...
}

SfxId AdvancedAudioEngine::loadEffect(const char* pszFilePath)
{
    return oAudioPlayer->loadEffect(pszFilePath);
}

SfxId AdvancedAudioEngine::loadMusic(const char* pszFilePath)
{
    return oAudioPlayer->loadMusic(pszFilePath);
}

void AdvancedAudioEngine::unload(SfxId soundId)
{
    oAudioPlayer->unload(soundId);
}

void AdvancedAudioEngine::pauseAll()
{
    oAudioPlayer->pauseAll();
}

void AdvancedAudioEngine::resumeAll()
{
    oAudioPlayer->resumeAll();
}

void AdvancedAudioEngine::stopAll()
{
    oAudioPlayer->stopAll();
}

float AdvancedAudioEngine::getVolume()
{
    return oAudioPlayer->getVolume();
}

void AdvancedAudioEngine::setVolume(float volume)
{
    oAudioPlayer->setVolume(volume);
}

SfxInstanceId AdvancedAudioEngine::play(SfxId soundId)
{
    return oAudioPlayer->play(soundId);
}

void AdvancedAudioEngine::stop(SfxInstanceId soundId)
{
    oAudioPlayer->stop(soundId);
}

void AdvancedAudioEngine::pause(SfxInstanceId soundId)
{
    oAudioPlayer->pause(soundId);
}

void AdvancedAudioEngine::resume(SfxInstanceId soundId)
{
    oAudioPlayer->resume(soundId);
}

bool AdvancedAudioEngine::isPlaying(SfxInstanceId soundId)
{
    return oAudioPlayer->isPlaying(soundId);
}

void AdvancedAudioEngine::setLoopCount(SfxInstanceId soundId, unsigned int loopCount)
{
    oAudioPlayer->setLoopCount(soundId, loopCount);
}

unsigned int AdvancedAudioEngine::getLoopCount(SfxInstanceId soundId)
{
    return oAudioPlayer->getLoopCount(soundId);
}

void AdvancedAudioEngine::setVolume(SfxInstanceId soundId, float volume)
{
    oAudioPlayer->setVolume(soundId, volume);
}

float AdvancedAudioEngine::getVolume(SfxInstanceId soundId)
{
    return oAudioPlayer->getVolume(soundId);
}

void AdvancedAudioEngine::setPitch(SfxInstanceId soundId, float pitch)
{
    oAudioPlayer->setPitch(soundId, pitch);
}

float AdvancedAudioEngine::getPitch(SfxInstanceId soundId)
{
    return oAudioPlayer->getPitch(soundId);
}

void AdvancedAudioEngine::setPanning(SfxInstanceId soundId, float pan)
{
    oAudioPlayer->setPanning(soundId, pan);
}

float AdvancedAudioEngine::getPanning(SfxInstanceId soundId)
{
    return oAudioPlayer->getPanning(soundId);
}

void AdvancedAudioEngine::setFadeInDuration(SfxInstanceId soundId, float fadeInDurationMs)
{
    oAudioPlayer->setFadeInDuration(soundId, fadeInDurationMs);
}

void AdvancedAudioEngine::setFadeOutDuration(SfxInstanceId soundId, float fadeOutDurationMs)
{
    oAudioPlayer->setFadeOutDuration(soundId, fadeOutDurationMs);
}

void AdvancedAudioEngine::seek(SfxInstanceId soundId, float position)
{
    oAudioPlayer->seek(soundId, position);
}

float AdvancedAudioEngine::position(SfxInstanceId soundId)
{
    return oAudioPlayer->position(soundId);
}

void AdvancedAudioEngine::setAudioEventListener(AudioEventListener *listener)
{
    oAudioPlayer->setAudioEventListener(listener);
}

void AdvancedAudioEngine::removeAudioEventListener()
{
    oAudioPlayer->removeAudioEventListener();
}

SfxId AdvancedAudioEngine::sfxIdForFile(const char* pszFilePath)
{
    return oAudioPlayer->sfxIdForFile(pszFilePath);
}

SfxInstanceId AdvancedAudioEngine::sfxInstanceIdForSfxId(SfxId soundId)
{
    return oAudioPlayer->sfxInstanceIdForSfxId(soundId);
}

SfxInstanceId AdvancedAudioEngine::getActiveSfxInstanceId()
{
    return oAudioPlayer->getActiveSfxInstanceId();
}

} // end of namespace CocosDenshion
