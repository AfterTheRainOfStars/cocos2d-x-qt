/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "QtAudioPlayer.h"
#include "pullaudioout.h"
#include "pushaudioout.h"
#include "CCFileUtils.h"
#include <QDebug>

using namespace GE;
USING_NS_CC;

namespace CocosDenshion {

static unsigned int _Hash(const char *key)
{
    unsigned int len = strlen(key);
    const char *end=key+len;
    unsigned int hash;

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (unsigned int) (unsigned char) toupper(*key);
    }
    return (hash);
}

static QString fullPathFromRelativePath(const char *pszRelativePath)
{
    QString strRet="";
    int len = strlen(pszRelativePath);
    if (pszRelativePath == NULL || len <= 0)
    {
        return strRet;
    }

    if (len > 1 && pszRelativePath[0] == '/')
    {
        strRet = pszRelativePath;
    }
    else
    {
        strRet = CCFileUtils::fullPathFromRelativePath(pszRelativePath);
    }
    return strRet;
}

QtAudioPlayer* QtAudioPlayer::sharedPlayer()
{
    static QtAudioPlayer s_SharedPlayer;
    return &s_SharedPlayer;
}

QtAudioPlayer::QtAudioPlayer() :
    m_mixer(NULL),
    m_audioOut(NULL),
    m_music(NULL),
    m_musicId(NULL),
    m_effectsVolume(0.7f),
    m_musicVolume(0.7f)
#ifndef USE_VORBIS_SOURCE
   ,m_musicBuffer(NULL)
#endif
{
    init();
}

void QtAudioPlayer::init()
{
    m_mixer = new AudioMixer(this);

#ifdef Q_OS_SYMBIAN
    m_audioOut = new PullAudioOut(m_mixer, this);
#else
    m_audioOut = new PushAudioOut(m_mixer, this);
#endif
}

void QtAudioPlayer::close()
{
    int i;

    delete m_mixer;
    m_mixer = NULL;

    delete m_audioOut;
    m_audioOut = NULL;

    for (i = 0; i < m_effectInstances.count(); i++)
        if (!m_effectInstances[i].m_ptr.isNull())
            delete m_effectInstances[i].m_ptr;

    m_effectInstances.clear();

    for (i = 0; i < m_effects.count(); i++)
        if (!m_effects[i].m_ptr.isNull())
            delete m_effects[i].m_ptr;

    m_effects.clear();
}

QtAudioPlayer::~QtAudioPlayer()
{
    close();
}

AudioBuffer *QtAudioPlayer::findBuffer(unsigned int hash)
{
    foreach(HashedPointer ptr, m_effects)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
            continue;
        }
        else if (hash == ptr.m_hash)
        {
            return (AudioBuffer*)(ptr.m_ptr.data());
        }
    }

    return NULL;
}

AudioBufferPlayInstance *QtAudioPlayer::findBufferInstance(unsigned int hash)
{
    foreach(HashedPointer ptr, m_effectInstances)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
        }
        else if (hash == ptr.m_hash)
        {
            return qobject_cast<AudioBufferPlayInstance*>(ptr.m_ptr);
        }
    }

    return NULL;
}

void QtAudioPlayer::preloadBackgroundMusic(const char* pszFilePath)
{
    qDebug() << "preloadBackgroundMusic" << pszFilePath;
    if (m_music)
    {
        m_mixer->removeAudioSource(m_music);
        delete m_music;
        m_music = NULL;
        m_musicId = 0;
    }

#ifdef USE_VORBIS_SOURCE
    m_music = new VorbisSource(fullPathFromRelativePath(pszFilePath));
#else
    if (m_musicBuffer)
    {
        delete m_musicBuffer;
        m_musicBuffer = NULL;
    }
    m_musicBuffer = AudioBuffer::load(fullPathFromRelativePath(pszFilePath));
#endif
    m_musicId = _Hash(pszFilePath);
    m_mixer->addAudioSource(m_music);
}

void QtAudioPlayer::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    qDebug() << "playBackgroundMusic" << pszFilePath << bLoop;
    unsigned int hash = _Hash(pszFilePath);
    if (m_musicId != hash)
    {
        preloadBackgroundMusic(pszFilePath);
    }
#ifdef USE_VORBIS_SOURCE
    m_music->play();
#else
    m_music = m_musicBuffer->playWithMixer(*m_mixer);
    m_music->setDestroyWhenFinished(false);
#endif
    m_music->setLeftVolume(m_musicVolume);
    m_music->setRightVolume(m_musicVolume);
    if (bLoop)
        m_music->setLoopCount(1<<30);
}

void QtAudioPlayer::stopBackgroundMusic(bool bReleaseData)
{
    qDebug() << "stopBackgroundMusic" << "Release: " << bReleaseData;
    if (!m_music)
        return;

    m_music->seek(0);
    m_music->stop();

    if (bReleaseData)
    {
        m_mixer->removeAudioSource(m_music);
        delete m_music;
        m_music = NULL;
        m_musicId = 0;
    }
}

void QtAudioPlayer::pauseBackgroundMusic()
{
    qDebug() << "pauseBackgroundMusic";
#ifdef USE_VORBIS_SOURCE
    if (m_music)
        m_music->stop();
#else
    if (m_music)
        m_music->pause();
#endif
}

void QtAudioPlayer::resumeBackgroundMusic()
{
    qDebug() << "resumeBackgroundMusic";
#ifdef USE_VORBIS_SOURCE
    if (m_music)
        m_music->play();
#else
    if (m_music)
        m_music->resume();
#endif
}

void QtAudioPlayer::rewindBackgroundMusic()
{
    qDebug() << "rewindBackgroundMusic";
    if (m_music)
        m_music->seek(0);
}

bool QtAudioPlayer::willPlayBackgroundMusic()
{
    qDebug() << "willPlayBackgroundMusic";
    return false;
}

bool QtAudioPlayer::isBackgroundMusicPlaying()
{
    qDebug() << "isBackgroundMusicPlaying";
    if (!m_music)
        return false;

    return (!m_music->isFinished());
}

float QtAudioPlayer::getBackgroundMusicVolume()
{
    qDebug() << "getBackgroundMusicVolume";
    return m_musicVolume;
}

void QtAudioPlayer::setBackgroundMusicVolume(float volume)
{
    qDebug() << "setBackgroundMusicVolume" << volume;

    if (volume > 1.0f)
    {
        volume = 1.0f;
    }
    else if (volume < 0.0f)
    {
        volume = 0.0f;
    }

    m_musicVolume = volume;

    if(m_music)
    {
        m_music->setLeftVolume(volume);
        m_music->setRightVolume(volume);
    }
}

// for sound effects
float QtAudioPlayer::getEffectsVolume()
{
    qDebug() << "getEffectsVolume" << m_effectsVolume;
    return m_effectsVolume;
}

void QtAudioPlayer::setEffectsVolume(float volume)
{
    qDebug() << "setEffectsVolume" << volume;
    if (volume > 1.0f)
    {
        volume = 1.0f;
    }
    else if (volume < 0.0f)
    {
        volume = 0.0f;
    }

    m_effectsVolume = volume;

    foreach (HashedPointer ptr, m_effectInstances)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
        }
        else
        {
            AudioBufferPlayInstance *instance =
                qobject_cast<AudioBufferPlayInstance*>(ptr.m_ptr);
            if (instance)
            {
                instance->setLeftVolume(m_effectsVolume);
                instance->setRightVolume(m_effectsVolume);
            }
        }
    }
}

unsigned int QtAudioPlayer::playEffect(const char* pszFilePath, bool bLoop)
{
    qDebug() << "playEffect" << pszFilePath << "loop " << bLoop;

    unsigned int hash = _Hash(pszFilePath);

    AudioBuffer *buffer = findBuffer(hash);
    if (!buffer)
    {
        preloadEffect(pszFilePath);
        buffer = findBuffer(hash);
    }

    // Not loaded successfully, or already playing
    if (!buffer || findBufferInstance(hash))
        return hash;

    AudioBufferPlayInstance *inst = buffer->playWithMixer(*m_mixer);
    inst->setLeftVolume(m_effectsVolume);
    inst->setRightVolume(m_effectsVolume);
    if (bLoop)
        inst->setLoopCount(1<<30);

    HashedPointer buf;
    buf.m_ptr = inst;
    buf.m_hash = hash;
    m_effectInstances.append(buf);

    return hash;
}

void QtAudioPlayer::stopEffect(unsigned int nSoundId)
{
    qDebug() << "stopEffect" << nSoundId;

    foreach(HashedPointer ptr, m_effectInstances)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
        }
        else if (nSoundId == ptr.m_hash)
        {
            AudioBufferPlayInstance *buffer =
                qobject_cast<AudioBufferPlayInstance*>(ptr.m_ptr);
            if (buffer)
                buffer->stop();

            m_effectInstances.removeOne(ptr);
        }
    }
}

void QtAudioPlayer::pauseEffect(unsigned int uSoundId)
{
    qDebug() << "pauseEffect" << uSoundId;
    AudioBufferPlayInstance *buffer = findBufferInstance(uSoundId);
    if (buffer)
        buffer->pause();
}

void QtAudioPlayer::pauseAllEffects()
{
    qDebug() << "pauseAllEffects";
    foreach (HashedPointer ptr, m_effectInstances)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
        }
        else
        {
            AudioBufferPlayInstance *instance =
                qobject_cast<AudioBufferPlayInstance*>(ptr.m_ptr);
            if (instance)
                instance->pause();
        }
    }
}

void QtAudioPlayer::resumeEffect(unsigned int uSoundId)
{
    qDebug() << "resumeEffect" << uSoundId;
    AudioBufferPlayInstance *buffer = findBufferInstance(uSoundId);
    if (buffer)
        buffer->resume();
}

void QtAudioPlayer::resumeAllEffects()
{
    qDebug() << "resumeAllEffects";
    foreach (HashedPointer ptr, m_effectInstances)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
        }
        else
        {
            AudioBufferPlayInstance *instance =
                qobject_cast<AudioBufferPlayInstance*>(ptr.m_ptr);
            if (instance)
                instance->resume();
        }
    }
}

void QtAudioPlayer::stopAllEffects()
{
    qDebug() << "stopAllEffects";
    foreach (HashedPointer ptr, m_effectInstances)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effects.removeOne(ptr);
        }
        else
        {
            AudioBufferPlayInstance *instance =
                qobject_cast<AudioBufferPlayInstance*>(ptr.m_ptr);
            if (instance)
                instance->stop();
        }
    }

    m_effectInstances.clear();
}

void QtAudioPlayer::preloadEffect(const char* pszFilePath)
{
    qDebug() << "preloadEffect" << pszFilePath;
    unsigned int hash = _Hash(pszFilePath);

    AudioBuffer *buffer = findBuffer(hash);
    if (!buffer)
    {
        HashedPointer buf;
        buf.m_ptr = AudioBuffer::load(fullPathFromRelativePath(pszFilePath));
        buf.m_hash = hash;
        m_effects.append(buf);
    }
}

void QtAudioPlayer::unloadEffect(const char* pszFilePath)
{
    qDebug() << "unloadEffect" << pszFilePath;
    unsigned int hash = _Hash(pszFilePath);
    stopEffect(hash);

    foreach(HashedPointer ptr, m_effects)
    {
        if (ptr.m_ptr.isNull())
        {
            m_effectInstances.removeOne(ptr);
            m_effects.removeOne(ptr);
        }
        else if (hash == ptr.m_hash)
        {
            delete ptr.m_ptr;
            m_effects.removeOne(ptr);
        }
    }
}

} /* namespace CocosDenshion */
