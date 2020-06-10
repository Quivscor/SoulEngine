#include "Listener.h"

Listener::Listener()
{
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 }; // 'at' vector, 'up' vector, default values
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

Listener::~Listener()
{
}