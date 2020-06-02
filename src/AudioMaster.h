#pragma once
#include <windows.h>
#include <al.h>
#include <alc.h>
#include <list>

static class AudioMaster
{
public:
	AudioMaster();
	~AudioMaster();
	void Init();
	void Clear();
	ALuint GenBuffer(const char *name);

	std::list<ALuint> buffers;

	ALCdevice *device;
	ALCcontext *context;
};
