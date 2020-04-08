#pragma once
#include "Core.h"
#include "Event.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

class IEventListener
{
	using EventCallbackFn = std::function<void(Event&)>;

public:
	EventCallbackFn EventCallback;

	void SetEventCallback(const EventCallbackFn& callback) { EventCallback = callback; }
	virtual void OnEvent(Event& e) = 0;
};
