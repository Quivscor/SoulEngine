#include "Source.h"

Source::Source()
{
	alGenSources(1, &sourceId);
	alSourcef(sourceId, AL_GAIN, 1); // volume
	alSourcef(sourceId, AL_PITCH, 1);
	alSource3f(sourceId, AL_POSITION, 0, 0, 0);
	alSourcei(sourceId, AL_LOOPING, false);
}

void Source::Play(ALuint buffer)
{
	alSourcei(sourceId, AL_BUFFER, buffer);
	alSourcePlay(sourceId);
}

void Source::Pause()
{
	if (paused)
		alSourcePlay(sourceId);
	else
		alSourcePause(sourceId);

	paused = !paused;
}

void Source::SetLooping(bool isLooped)
{
	// defualt is not looping
	alSourcei(sourceId, AL_LOOPING, isLooped);
}

void Source::SetVolume(float volume)
{
	// 1.0 is default, 0 is silence
	alSourcef(sourceId, AL_GAIN, volume);
}

Source::~Source()
{
	alDeleteSources(1, &sourceId);
}

