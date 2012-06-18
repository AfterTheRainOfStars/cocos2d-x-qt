/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef ADVANCED_AUDIOENGINE_H_
#define ADVANCED_AUDIOENGINE_H_

#include "Export.h"
#include <stddef.h>

namespace CocosDenshion {

typedef unsigned int SfxId;
typedef unsigned int SfxInstanceId;

class AudioEventListener {
public:
    virtual void effectEnded(SfxInstanceId soundId) = 0;
};

class EXPORT_DLL AdvancedAudioEngine {
public:
    AdvancedAudioEngine();
    virtual ~AdvancedAudioEngine();

    static AdvancedAudioEngine* sharedEngine();
    static void end();

    SfxId loadEffect(const char* pszFilePath);
    SfxId loadMusic(const char* pszFilePath);
    void unload(SfxId soundId);

    void pauseAll();
    void resumeAll();
    void stopAll();

    float getVolume();
    void setVolume(float volume);

    SfxInstanceId play(SfxId soundId);
    void stop(SfxInstanceId soundId);
    void pause(SfxInstanceId soundId);
    void resume(SfxInstanceId soundId);
    bool isPlaying(SfxInstanceId soundId);

    void setLoopCount(SfxInstanceId soundId, unsigned int loopCount);
    unsigned int getLoopCount(SfxInstanceId soundId);

    void setVolume(SfxInstanceId soundId, float volume);
    float getVolume(SfxInstanceId soundId);

    void setPitch(SfxInstanceId soundId, float pitch);
    float getPitch(SfxInstanceId soundId);

    void setPanning(SfxInstanceId soundId, float pan);
    float getPanning(SfxInstanceId soundId);

    void setFadeInDuration(SfxInstanceId soundId, float fadeInDurationMs);
    void setFadeOutDuration(SfxInstanceId soundId, float fadeOutDurationMs);

    void seek(SfxInstanceId soundId, float position);
    float position(SfxInstanceId soundId);

    void setAudioEventListener(AudioEventListener *listener);
    void removeAudioEventListener();

    SfxId sfxIdForFile(const char *pszFilePath);
    SfxInstanceId sfxInstanceIdForSfxId(SfxId soundId);
    SfxInstanceId getActiveSfxInstanceId();
};
}

#endif /* ADVANCED_AUDIOENGINE_H_ */
