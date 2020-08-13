#pragma once

#include "Razer/Core.h"
#include "Event.h"

namespace rz {

	class RZAPI MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float x, float y) 
		:m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		static EventType GetStaticType() { return EventType::MouseMove; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseMove"; }
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		virtual std::string ToString() const override {
			std::stringstream  ss;
			ss << "MouseMove: " << m_MouseX << " " << m_MouseY;
			return ss.str();
		}

	private:
		float m_MouseX;
		float m_MouseY;
	};

	
	class RZAPI MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		static EventType GetStaticType() { return EventType::MouseScroll; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseScroll"; }
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScroll: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

	private:
		float  m_XOffset;
		float m_YOffset;
	};


	class RZAPI MouseButtonEvent : public Event {
	public:
		int GetMouseButton() const { return m_Button; }
		virtual int GetCategoryFlags() const override 
		{ return EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton; }

	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	
	class RZAPI MouseButtonPressEvent : public MouseButtonEvent {
	public:
		MouseButtonPressEvent(int button)
			: MouseButtonEvent(button) {}

		static EventType GetStaticType() { return EventType::MouseButtonPress; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonPress"; }
		
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPress: " << GetMouseButton();
			return ss.str();
		}
	};


	class RZAPI MouseButtonReleaseEvent : public MouseButtonEvent {
	public:
		MouseButtonReleaseEvent(int button)
			: MouseButtonEvent(button) {}

		static EventType GetStaticType() { return EventType::MouseButtonRelease; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonRelease"; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonRelease: " << GetMouseButton();
			return ss.str();
		}
	};
}