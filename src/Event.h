#pragma once

class Transform;

enum class EventType {
	None = 0,
	LogEvent,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	BindTransform,
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }

class Event
{
public:
	bool Handled = false;

	virtual EventType GetEventType() const = 0;
	virtual std::string ToString() const = 0;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event) : m_Event(event)
	{ }

	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled = func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};

class LogEvent : public Event
{
public:
	LogEvent(std::string message) : m_Message(message) { }
	virtual std::string ToString() const override { return m_Message; }
	
	EVENT_CLASS_TYPE(LogEvent)

private:
	std::string m_Message;
};

class BindTransform : public Event
{
public:
	BindTransform(std::shared_ptr<Transform> transform) : m_Component(transform) { }
	virtual std::string ToString() const override { return "Transform pointer."; }

	EVENT_CLASS_TYPE(BindTransform)

	std::shared_ptr<Transform> GetTransformComponent() { return m_Component; }

private:
	std::shared_ptr<Transform> m_Component;
};
