#pragma once
#include "../Core.h"

#include <sstream>

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus,
	KeyDown, KeyUp,
	MouseMove, MouseButtonDown, MouseButtonUp
};

enum EventCategory
{
	None = 0,
	EventCategoryWindow = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryInput = BIT(3),
	EventCategoryMouse = BIT(4),
	EventCategoryMouseButton = BIT(5)
};

class Event
{
public:
	virtual ~Event() = default;

	bool Handled = false;

	virtual EventType getType() const = 0;
	virtual const char* getName() const = 0;
	virtual int getCategoryFlags() const = 0;

	virtual std::string toString() const { return ""; };

	bool isInCategory(EventCategory category)
	{
		return getCategoryFlags() & category;
	}
};

class KeyEvent : public Event
{
public:
	int getKeyCode() const { return m_keyCode; }

	int getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput;  }

protected:
	KeyEvent(const int keyCode) : m_keyCode(keyCode) {}
	int m_keyCode;
};

class KeyDownEvent : public KeyEvent
{
public:
	KeyDownEvent(const int keyCode, bool isRepeat = false)
		: KeyEvent(keyCode), m_isRepeat(isRepeat) {}

	bool isRepeat() const { return m_isRepeat; }

	EventType getType() const override { return EventType::KeyDown; }
	const char* getName() const override { return "KeyDownEvent"; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName()  << " - Key Code: " << m_keyCode << " - Is Repeat: " << m_isRepeat;
		return ss.str();
	}

private:
	bool m_isRepeat;
};

class KeyUpEvent : public KeyEvent
{
public:
	KeyUpEvent(const int keyCode)
		: KeyEvent(keyCode) {} 

	EventType getType() const override { return EventType::KeyUp; }
	const char* getName() const override { return "KeyUpEvent"; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName() << " - Key Code: " << m_keyCode;
		return ss.str();
	}
};

class MouseMoveEvent: public Event
{
public:
	MouseMoveEvent(double xPos, double yPos)
		: m_xPos(xPos), m_yPos(yPos) {}

	EventType getType() const override { return EventType::MouseMove; }
	const char* getName() const override { return "MouseMoveEvent"; }
	int getCategoryFlags() const override { return EventCategoryMouse; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName() << " - Pos (" << m_xPos << ", " << m_yPos << ")";
		return ss.str();
	}

private:
	double m_xPos;
	double m_yPos;
};

class MouseButtonEvent : public Event
{
public:
	int getKeyCode() const { return m_keyCode; }

	int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryMouseButton; }

protected:
	MouseButtonEvent(const int keyCode) : m_keyCode(keyCode) {}
	int m_keyCode;
};

class MouseButtonDownEvent : public MouseButtonEvent
{
public:
	MouseButtonDownEvent(const int keyCode)
		: MouseButtonEvent(keyCode) {}

	EventType getType() const override { return EventType::MouseButtonDown; }
	const char* getName() const override { return "MouseButtonDownEvent"; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName() << " - KeyCode " << m_keyCode;
		return ss.str();
	}
};

class MouseButtonUpEvent : public MouseButtonEvent
{
public:
	MouseButtonUpEvent(const int keyCode) : MouseButtonEvent(keyCode) {} 

	EventType getType() const override { return EventType::MouseButtonUp; }
	const char* getName() const override { return "MouseButtonUpEvent"; }
	
	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName() << " - KeyCode " << m_keyCode;
		return ss.str();
	}
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;
	
	EventType getType() const override { return EventType::WindowClose; }
	const char* getName() const override { return "WindowCloseEvent"; }
	int getCategoryFlags() const override { return EventCategoryWindow;  }

	std::string toString() const override
	{
		return getName();
	}
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height)
		: m_width(width), m_height(height) {} 

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	EventType getType() const override { return EventType::WindowResize; }
	const char* getName() const override { return "WindowResizeEvent"; }
	int getCategoryFlags() const override { return EventCategoryWindow; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName() << " - new size (" << m_width << ", " << m_height << ")";
		return ss.str();
	}

private:
	int m_width;
	int m_height;
};

class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent(bool focused)
		: m_focused(focused) {}

	int getFocused() const { return m_focused; }

	EventType getType() const override { return EventType::WindowFocus; }
	const char* getName() const override { return "WindowFocusEvent"; }
	int getCategoryFlags() const override { return EventCategoryWindow; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << getName() << " - is focused: " << m_focused;
		return ss.str();
	}

private:
	bool m_focused;
};