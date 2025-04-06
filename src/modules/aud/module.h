#pragma once
#include "../moduleLib.h"

struct sAudioClip {
    void* internal;
};

struct sAudioSource {
    void* internal;
};

namespace audio {
    typedef void (*Init)();
    typedef sAudioClip (*LoadAudioClip)(const char* path);
    typedef sAudioClip (*LoadAudioClipFromMemory)(const char* name, const void* data, size_t size);
    typedef sAudioSource (*CreateAudioSource)(sAudioClip clip);
    typedef void (*PlayAudioSource)(sAudioSource source);
    typedef void (*StopAudioSource)(sAudioSource source);
    typedef void (*SetAudioSourcePosition)(sAudioSource source, float x, float y, float z);
    typedef void (*SetAudioSourceVelocity)(sAudioSource source, float x, float y, float z);
    typedef void (*SetAudioSourcePitch)(sAudioSource source, float pitch);
    typedef void (*SetAudioSourceGain)(sAudioSource source, float gain);
    typedef void (*SetAudioSourceLooping)(sAudioSource source, bool looping);
    typedef void (*SeekAudioSourceSamples)(sAudioSource source, int samples);
    typedef int (*GetAudioSourceSampleCount)(sAudioSource source);
    typedef int (*GetAudioSourceSamplePos)(sAudioSource source);
    typedef int (*GetAudioSourceSampleRate)(sAudioSource source);
    typedef void (*DestroyAudioClip)(sAudioClip clip);
    typedef void (*DestroyAudioSource)(sAudioSource source);
    typedef void (*Destroy)();

}

struct AudioModule : public Module {
    audio::Init init;
    audio::LoadAudioClip loadAudioClip;
    audio::LoadAudioClipFromMemory loadAudioClipFromMemory;
    audio::CreateAudioSource createAudioSource;
    audio::PlayAudioSource playAudioSource;
    audio::StopAudioSource stopAudioSource;
    audio::SetAudioSourcePosition setAudioSourcePosition;
    audio::SetAudioSourceVelocity setAudioSourceVelocity;
    audio::SetAudioSourcePitch setAudioSourcePitch;
    audio::SetAudioSourceGain setAudioSourceGain;
    audio::SetAudioSourceLooping setAudioSourceLooping;
    audio::SeekAudioSourceSamples seekAudioSourceSamples;
    audio::GetAudioSourceSampleCount getAudioSourceSampleCount;
    audio::GetAudioSourceSamplePos getAudioSourceSamplePos;
    audio::GetAudioSourceSampleRate getAudioSourceSampleRate;
    audio::DestroyAudioClip destroyAudioClip;
    audio::DestroyAudioSource destroyAudioSource;
    audio::Destroy destroy;

    inline void seekAudioSourceSeconds(sAudioSource source, float seconds) {
        seekAudioSourceSamples(source, seconds * getAudioSourceSampleRate(source));
    }

    inline void seekAudioSourcePercent(sAudioSource source, float percent) {
        seekAudioSourceSamples(source, percent * getAudioSourceSampleCount(source));
    }

    inline float getAudioSourcePercent(sAudioSource source) {
        return (float)getAudioSourceSampleCount(source) / getAudioSourceSampleRate(source);
    }

    inline float getAudioSourceSeconds(sAudioSource source) {
        return (float)getAudioSourceSampleCount(source) / getAudioSourceSampleRate(source);
    }

    inline explicit AudioModule(const char* dylib) : Module(dylib, "aud") {
        init = (audio::Init)lib.getSymbol("init");
        loadAudioClip = (audio::LoadAudioClip)lib.getSymbol("loadAudioClip");
        loadAudioClipFromMemory = (audio::LoadAudioClipFromMemory)lib.getSymbol("loadAudioClipFromMemory");
        createAudioSource = (audio::CreateAudioSource)lib.getSymbol("createAudioSource");
        playAudioSource = (audio::PlayAudioSource)lib.getSymbol("playAudioSource");
        stopAudioSource = (audio::StopAudioSource)lib.getSymbol("stopAudioSource");
        setAudioSourcePosition = (audio::SetAudioSourcePosition)lib.getSymbol("setAudioSourcePosition");
        setAudioSourceVelocity = (audio::SetAudioSourceVelocity)lib.getSymbol("setAudioSourceVelocity");
        setAudioSourcePitch = (audio::SetAudioSourcePitch)lib.getSymbol("setAudioSourcePitch");
        setAudioSourceGain = (audio::SetAudioSourceGain)lib.getSymbol("setAudioSourceGain");
        setAudioSourceLooping = (audio::SetAudioSourceLooping)lib.getSymbol("setAudioSourceLooping");
        seekAudioSourceSamples = (audio::SeekAudioSourceSamples)lib.getSymbol("seekAudioSourceSamples");
        getAudioSourceSampleCount = (audio::GetAudioSourceSampleCount)lib.getSymbol("getAudioSourceSampleCount");
        getAudioSourceSamplePos = (audio::GetAudioSourceSamplePos)lib.getSymbol("getAudioSourceSamplePos");
        getAudioSourceSampleRate = (audio::GetAudioSourceSampleRate)lib.getSymbol("getAudioSourceSampleRate");
        destroyAudioClip = (audio::DestroyAudioClip)lib.getSymbol("destroyAudioClip");
        destroyAudioSource = (audio::DestroyAudioSource)lib.getSymbol("destroyAudioSource");
        destroy = (audio::Destroy)lib.getSymbol("destroy");
    }
};