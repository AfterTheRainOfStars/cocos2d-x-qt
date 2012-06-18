/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef ADVANCEDAUDIOPLAYER_H_
#define ADVANCEDAUDIOPLAYER_H_

#include "AdvancedAudioEngine.h"

namespace CocosDenshion {

class AdvancedAudioPlayer {
public:
    virtual SfxId loadEffect(const char* pszFilePath) = 0;
    virtual SfxId loadMusic(const char* pszFilePath) = 0;
    virtual void unload(SfxId soundId) = 0;

    virtual void pauseAll() = 0;
    virtual void resumeAll() = 0;
    virtual void stopAll() = 0;

    virtual float getVolume() = 0;
    virtual void setVolume(float volume) = 0;

    virtual SfxInstanceId play(SfxId soundId) = 0;
    virtual void stop(SfxInstanceId soundId) = 0;
    virtual void pause(SfxInstanceId soundId) = 0;
    virtual void resume(SfxInstanceId soundId) = 0;
    virtual bool isPlaying(SfxInstanceId soundId) = 0;

    virtual void setLoopCount(SfxInstanceId soundId, unsigned int loopCount) = 0;
    virtual unsigned int getLoopCount(SfxInstanceId soundId) = 0;

    virtual void setVolume(SfxInstanceId soundId, float volume) = 0;
    virtual float getVolume(SfxInstanceId soundId) = 0;

    virtual void setPitch(SfxInstanceId soundId, float pitch) = 0;
    virtual float getPitch(SfxInstanceId soundId) = 0;

    virtual void setPanning(SfxInstanceId soundId, float pan) = 0;
    virtual float getPanning(SfxInstanceId soundId) = 0;

    virtual void setFadeInDuration(SfxInstanceId soundId, float fadeInDurationMs) = 0;
    virtual void setFadeOutDuration(SfxInstanceId soundId, float fadeOutDurationMs) = 0;

    virtual void seek(SfxInstanceId soundId, float position) = 0;
    virtual float position(SfxInstanceId soundId) = 0;

    virtual void setAudioEventListener(AudioEventListener *listener) = 0;
    virtual void removeAudioEventListener() = 0;

    virtual SfxId sfxIdForFile(const char* pszFilePath) = 0;
    virtual SfxInstanceId sfxInstanceIdForSfxId(SfxId soundId) = 0;
    virtual SfxInstanceId getActiveSfxInstanceId() = 0;
};
}


#endif /* ADVANCEDAUDIOPLAYER_H_ */
