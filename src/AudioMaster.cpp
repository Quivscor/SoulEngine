#include "AudioMaster.h"


AudioMaster::AudioMaster()
{
	device = alcOpenDevice(NULL); // defualt implementation 
	if(device)
	{
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
	}
	alGetError();
}


AudioMaster::~AudioMaster()
{

}

void AudioMaster::Init()
{

}

ALuint AudioMaster::GenBuffer(const char *name)
{
	FILE *fp = NULL;
	fp = fopen(name, "rb"); //  r - read, b - binary mode
	if (fp == NULL)
		return -1;

	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;

	fread(type, sizeof(char), 4, fp); //RIFF header
	fread(&size, sizeof(DWORD), 1, fp);
	fread(type, sizeof(char), 4, fp); //WAVE 
	fread(type, sizeof(char), 4, fp); //fmt - sub chunk of WAVE, specyfing the data

	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
		return -1; //missing data

	fread(&dataSize, sizeof(DWORD), 1, fp);

	unsigned char* buf = new unsigned char[dataSize];
	fread(buf, sizeof(BYTE), dataSize, fp);

	ALuint buffer;
	alGenBuffers(1, &buffer);

	ALuint frequency = sampleRate;
	ALenum format = 0;

	if (bitsPerSample == 8)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	else if (bitsPerSample == 16)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}

	alBufferData(buffer, format, buf, dataSize, frequency);

	buffers.push_back(buffer);
	return buffer;
}

void AudioMaster::Clear()
{
	for each (auto buffer in buffers)
	{
		alDeleteBuffers(1, &buffer);
	}
	alcDestroyContext(context);
	alcCloseDevice(device);
}