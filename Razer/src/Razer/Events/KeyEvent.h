#pragma once

#include "Razer/Core.h"
#include "Event.h"

namespace rz {

	class RZAPI KeyEvent : public Event {
	public:
		int GetKeyCode() const { return m_KeyCode; }
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryKeyboard; }

	protected:
		KeyEvent(int keyCode)
			:m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class RZAPI KeyTypeEvent : public KeyEvent {
	public:
		KeyTypeEvent(int keyCode)
			: KeyEvent(keyCode) {}

		static EventType GetStaticType() { return EventType::KeyType; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyType"; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyType: " << GetKeyCode();
			return ss.str();
		}
	};


	class RZAPI KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		int GetRepeatCount() { return m_RepeatCount; }
		static EventType GetStaticType() { return EventType::KeyPress; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyPress"; }
		
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPress: " << GetKeyCode();
			return ss.str();
		}

	private:
		int m_RepeatCount;
	};


	class RZAPI KeyReleaseEvent : public KeyEvent {
	public:
		KeyReleaseEvent(int keyCode)
			: KeyEvent(keyCode) {}

		static EventType GetStaticType() { return EventType::KeyRelease; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyRelease"; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyRelease: " << GetKeyCode();
			return ss.str();
		}
	};
}
