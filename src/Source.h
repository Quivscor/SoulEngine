#pragma once
#include <al.h>
#include <alc.h>

class Source {
public:
	Source();
	~Source();
	void Play(ALuint);
	void SetVolume(float);
	void Pause();

	bool paused;
private:
	ALuint sourceId;
	
};