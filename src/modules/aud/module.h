#pragma once
#include "../moduleLib.h"

struct sAudioClip {
    void* internal;
};

struct sAudioSource {
    void* internal;
    float posX, posY, posZ;
    float velX, velY, velZ;
};

namespace audio {
    typedef void (*Init)();
    typedef sAudioClip (*LoadAudioClip)(const char* path);
    typedef sAudioSource (*CreateAudioSource)(sAudioClip clip);
    typedef void (*PlayAudioSource)(sAudioSource source);
    typedef void (*StopAudioSource)(sAudioSource source);
    typedef void (*SetAudioSourcePosition)(sAudioSource source, float x, float y, float z);
    typedef void (*SetAudioSourceVelocity)(sAudioSource source, float x, float y, float z);
    typedef void (*SetAudioSourcePitch)(sAudioSource source, float pitch);
    typedef void (*SetAudioSourceGain)(sAudioSource source, float gain);
    typedef void (*SetAudioSourceLooping)(sAudioSource source, bool looping);
    typedef void (*SeekAudioSourceSamples)(sAudioSource source, int samples);
    typedef int (*GetAudioSourceSamples)(sAudioSource source);
    typedef int (*GetAudioSourceSampleRate)(sAudioSource source);
    typedef void (*DestroyAudioClip)(sAudioClip clip);
    typedef void (*DestroyAudioSource)(sAudioSource source);
    typedef void (*Destroy)();

}

struct AudioModule : public Module {
    audio::Init init;
    audio::LoadAudioClip loadAudioClip;
    audio::CreateAudioSource createAudioSource;
    audio::PlayAudioSource playAudioSource;
    audio::StopAudioSource stopAudioSource;
    audio::SetAudioSourcePosition setAudioSourcePosition;
    audio::SetAudioSourceVelocity setAudioSourceVelocity;
    audio::SetAudioSourcePitch setAudioSourcePitch;
    audio::SetAudioSourceGain setAudioSourceGain;
    audio::SetAudioSourceLooping setAudioSourceLooping;
    audio::SeekAudioSourceSamples seekAudioSourceSamples;
    audio::GetAudioSourceSamples getAudioSourceSamples;
    audio::GetAudioSourceSampleRate getAudioSourceSampleRate;
    audio::DestroyAudioClip destroyAudioClip;
    audio::DestroyAudioSource destroyAudioSource;
    audio::Destroy destroy;

    inline void seekAudioSourceSeconds(sAudioSource source, float seconds) {
        seekAudioSourceSamples(source, seconds * getAudioSourceSampleRate(source));
    }

    inline void seekAudioSourcePercent(sAudioSource source, float percent) {
        seekAudioSourceSamples(source, percent * getAudioSourceSamples(source));
    }

    inline float getAudioSourcePercent(sAudioSource source) {
        return (float)getAudioSourceSamples(source) / getAudioSourceSampleRate(source);
    }

    inline float getAudioSourceSeconds(sAudioSource source) {
        return (float)getAudioSourceSamples(source) / getAudioSourceSampleRate(source);
    }

    inline explicit AudioModule(const char* dylib) : Module(dylib, "aud") {
        init = (audio::Init)lib.getSymbol("init");
        loadAudioClip = (audio::LoadAudioClip)lib.getSymbol("loadAudioClip");
        createAudioSource = (audio::CreateAudioSource)lib.getSymbol("createAudioSource");
        playAudioSource = (audio::PlayAudioSource)lib.getSymbol("playAudioSource");
        stopAudioSource = (audio::StopAudioSource)lib.getSymbol("stopAudioSource");
        setAudioSourcePosition = (audio::SetAudioSourcePosition)lib.getSymbol("setAudioSourcePosition");
        setAudioSourceVelocity = (audio::SetAudioSourceVelocity)lib.getSymbol("setAudioSourceVelocity");
        setAudioSourcePitch = (audio::SetAudioSourcePitch)lib.getSymbol("setAudioSourcePitch");
        setAudioSourceGain = (audio::SetAudioSourceGain)lib.getSymbol("setAudioSourceGain");
        setAudioSourceLooping = (audio::SetAudioSourceLooping)lib.getSymbol("setAudioSourceLooping");
        seekAudioSourceSamples = (audio::SeekAudioSourceSamples)lib.getSymbol("seekAudioSourceSamples");
        getAudioSourceSamples = (audio::GetAudioSourceSamples)lib.getSymbol("getAudioSourceSamples");
        getAudioSourceSampleRate = (audio::GetAudioSourceSampleRate)lib.getSymbol("getAudioSourceSampleRate");
        destroyAudioClip = (audio::DestroyAudioClip)lib.getSymbol("destroyAudioClip");
        destroyAudioSource = (audio::DestroyAudioSource)lib.getSymbol("destroyAudioSource");
        destroy = (audio::Destroy)lib.getSymbol("destroy");
    }
};