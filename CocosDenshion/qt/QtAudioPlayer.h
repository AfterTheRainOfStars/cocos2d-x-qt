/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef QTAUDIOPLAYER_H_
#define QTAUDIOPLAYER_H_

#include "AudioPlayer.h"
#include "audiomixer.h"
#include "audioout.h"
#include "vorbissource.h"
#include "audiobufferplayinstance.h"

#include <QVariant>
#include <QList>

using namespace GE;

namespace CocosDenshion {

class QtAudioPlayer : public QObject,
                      public AudioPlayer
{
    Q_OBJECT

public:
    QtAudioPlayer();
    virtual ~QtAudioPlayer();

    static QtAudioPlayer* sharedPlayer();

    virtual void close();

    /**
     @brief Preload background music
     @param pszFilePath The path of the background music file,or the FileName of T_SoundResInfo
     */
    virtual void preloadBackgroundMusic(const char* pszFilePath);

    /**
     @brief Play background music
     @param pszFilePath The path of the background music file,or the FileName of T_SoundResInfo
     @param bLoop Whether the background music loop or not
     */
    virtual void playBackgroundMusic(const char* pszFilePath, bool bLoop);

    /**
     @brief Stop playing background music
     @param bReleaseData If release the background music data or not.As default value is false
     */
    virtual void stopBackgroundMusic(bool bReleaseData);

    /**
     @brief Pause playing background music
     */
    virtual void pauseBackgroundMusic();

    /**
     @brief Resume playing background music
     */
    virtual void resumeBackgroundMusic();

    /**
     @brief Rewind playing background music
     */
    virtual void rewindBackgroundMusic();

    virtual bool willPlayBackgroundMusic();

    /**
     @brief Whether the background music is playing
     @return If is playing return true,or return false
     */
    virtual bool isBackgroundMusicPlaying();

    // properties
    /**
     @brief The volume of the background music max value is 1.0,the min value is 0.0
     */
    virtual float getBackgroundMusicVolume();

    /**
     @brief set the volume of background music
     @param volume must be in 0.0~1.0
     */
    virtual void setBackgroundMusicVolume(float volume);

    /**
     @brief The volume of the effects max value is 1.0,the min value is 0.0
     */
    virtual float getEffectsVolume();

    /**
     @brief set the volume of sound effecs
     @param volume must be in 0.0~1.0
     */
    virtual void setEffectsVolume(float volume);

    // for sound effects
    /**
     @brief Play sound effect
     @param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
     @bLoop Whether to loop the effect playing, default value is false
     */
    virtual unsigned int playEffect(const char* pszFilePath, bool bLoop = false);

    /**
     @brief Stop playing sound effect
     @param nSoundId The return value of function playEffect
     */
    virtual void stopEffect(unsigned int nSoundId);

    /**
     @brief  		preload a compressed audio file
     @details	    the compressed audio will be decode to wave, then write into an
     internal buffer in SimpleaudioEngine
     */
    virtual void preloadEffect(const char* pszFilePath);

    /**
     @brief  		unload the preloaded effect from internal buffer
     @param[in]		pszFilePath		The path of the effect file,or the FileName of T_SoundResInfo
     */
    virtual void unloadEffect(const char* pszFilePath);

    /**
     @brief  		pause an effect identified by sound id
     @param[in]		uSoundId 	sound id
     */
    virtual void pauseEffect(unsigned int uSoundId);

    /**
     @brief  		pause all playing effects
     */
    virtual void pauseAllEffects();

    /**
     @brief  		resume an effect identified by sound id
     @param[in]		uSoundId 	sound id
     */
    virtual void resumeEffect(unsigned int uSoundId);

    /**
     @brief  		resume a effect identified by sound id
     */
    virtual void resumeAllEffects();

    /**
     @brief  		stop all playing effects
     */
    virtual void stopAllEffects();

private:
    void init();
    void checkFinishedEffects();

private:
    AudioMixer *m_mixer;
    AudioOut *m_audioOut;
    QHash<unsigned int, AudioBuffer*> m_effects;
    QHash<unsigned int, QPointer<AudioBufferPlayInstance> > m_effectInstances;
    unsigned int m_musicId;
    float m_effectsVolume;
    float m_musicVolume;
#ifdef USE_VORBIS_SOURCE
    QPointer<VorbisSource> m_music;
#else
    QPointer<AudioBufferPlayInstance> m_music;
    AudioBuffer *m_musicBuffer;
#endif
};

} /* namespace CocosDenshion */

#endif /* QTAUDIOPLAYER_H_ */
