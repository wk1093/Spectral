#include "module.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

sArenaAllocator* gArena = nullptr;

struct InternalAudioSource {
    ALuint source;
};

sTypedSmartArena<InternalAudioSource>* gAudioSourceArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return 1024; // Assume 1KB for now, this should be enough for initialization
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
    gAudioSourceArena = new sTypedSmartArena<InternalAudioSource>(arena->tracker, 1024 * 2); // 2KB for audio sources
}

struct GlobalAudioState {
    ALCdevice *device;
    ALCcontext *context;
};

GlobalAudioState gAudioState;

CEXPORT void init() {
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    /* Open and initialize a device */
    device = NULL;
    if(!device)
        device = alcOpenDevice(NULL);
    if(!device)
    {
        fprintf(stderr, "Could not open a device!\n");
        return;
    }

    ctx = alcCreateContext(device, NULL);
    if(ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if(ctx != NULL)
            alcDestroyContext(ctx);
        alcCloseDevice(device);
        fprintf(stderr, "Could not set a context!\n");
        return;
    }

    name = NULL;
    if(alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if(!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);

    gAudioState.device = device;
    gAudioState.context = ctx;
    printf("Audio device: \"%s\"\n", name);
}

CEXPORT void destroy() {
    if(gAudioState.context) {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(gAudioState.context);
    }
    if(gAudioState.device) {
        alcCloseDevice(gAudioState.device);
    }
    gAudioState.device = NULL;
    gAudioState.context = NULL;
}

struct InternalAudioClip {
    ALuint buffer;
};

CEXPORT sAudioClip loadAudioClip(const char* path) {
    ALenum format;
    ALsizei size, freq;
    ALboolean loop = AL_FALSE;
    ALvoid *data = NULL;

    ALCdevice *device = gAudioState.device;
    ALCcontext *ctx = gAudioState.context;

    if(!device || !ctx) {
        fprintf(stderr, "Audio device or context not initialized!\n");
        return {nullptr};
    }

    printf("Audio loading not implemented yet\n");
    return {nullptr};

    if(!data) {
        fprintf(stderr, "Could not load audio data from file: %s\n", path);
        return {nullptr};
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data, size, freq);

    // Free the loaded data after buffering
    free(data);

    InternalAudioClip* internal = gArena->allocate<InternalAudioClip>();
    internal->buffer = buffer;
    return {internal};
}

CEXPORT sAudioClip loadAudioClipFromMemory(const char* name, const void* data, size_t size) {
    ALenum format;
    ALsizei freq;
    ALboolean loop = AL_FALSE;

    ALCdevice *device = gAudioState.device;
    ALCcontext *ctx = gAudioState.context;

    if(!device || !ctx) {
        fprintf(stderr, "Audio device or context not initialized!\n");
        return {nullptr};
    }

    printf("Audio loading not implemented yet\n");
    return {nullptr};

    if(!data) {
        fprintf(stderr, "Could not load audio data from memory: %s\n", name);
        return {nullptr};
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data, size, freq);


    InternalAudioClip* internal = gArena->allocate<InternalAudioClip>();
    internal->buffer = buffer;
    return {internal};
}

CEXPORT sAudioSource createAudioSource(sAudioClip clip) {
    InternalAudioClip* internalClip = (InternalAudioClip*)clip.internal;
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, internalClip->buffer);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        fprintf(stderr, "Error creating audio source: %s\n", alGetString(error));
        return {nullptr};
    }
    InternalAudioSource* internalSource = gAudioSourceArena->allocate();
    internalSource->source = source;
    return {internalSource};
}

CEXPORT void playAudioSource(sAudioSource source) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alSourcePlay(internalSource->source);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        fprintf(stderr, "Error playing audio source: %s\n", alGetString(error));
    }
}
CEXPORT void stopAudioSource(sAudioSource source) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alSourceStop(internalSource->source);
}
CEXPORT void setAudioSourcePosition(sAudioSource source, float x, float y, float z) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    ALfloat position[] = {x, y, z};
    alSourcefv(internalSource->source, AL_POSITION, position);
}
CEXPORT void setAudioSourceVelocity(sAudioSource source, float x, float y, float z) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    ALfloat velocity[] = {x, y, z};
    alSourcefv(internalSource->source, AL_VELOCITY, velocity);
}
CEXPORT void setAudioSourcePitch(sAudioSource source, float pitch) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alSourcef(internalSource->source, AL_PITCH, pitch);
}
CEXPORT void setAudioSourceGain(sAudioSource source, float gain) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alSourcef(internalSource->source, AL_GAIN, gain);
}
CEXPORT void setAudioSourceLooping(sAudioSource source, bool looping) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alSourcei(internalSource->source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}
CEXPORT void seekAudioSourceSamples(sAudioSource source, int samples) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alSourcei(internalSource->source, AL_SAMPLE_OFFSET, samples);
}
CEXPORT int getAudioSourceSampleCount(sAudioSource source) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    ALint sizeInBytes;
    ALint channels;
    ALint bits;
    alGetBufferi(internalSource->source, AL_SIZE, &sizeInBytes);
    alGetBufferi(internalSource->source, AL_CHANNELS, &channels);
    alGetBufferi(internalSource->source, AL_BITS, &bits);
    return sizeInBytes / (channels * (bits / 8));
}
CEXPORT int getAudioSourceSamplePos(sAudioSource source) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    ALint samples;
    alGetSourcei(internalSource->source, AL_SAMPLE_OFFSET, &samples);
    return samples;
}
CEXPORT int getAudioSourceSampleRate(sAudioSource source) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    ALint freq;
    alGetBufferi(internalSource->source, AL_FREQUENCY, &freq);
    return freq;
}
CEXPORT void destroyAudioClip(sAudioClip clip) {
    InternalAudioClip* internalClip = (InternalAudioClip*)clip.internal;
    alDeleteBuffers(1, &internalClip->buffer);
}
CEXPORT void destroyAudioSource(sAudioSource source) {
    InternalAudioSource* internalSource = (InternalAudioSource*)source.internal;
    alDeleteSources(1, &internalSource->source);
    gAudioSourceArena->free(internalSource);
}
