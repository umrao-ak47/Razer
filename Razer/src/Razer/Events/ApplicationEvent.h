#pragma once

#include "Razer/Core.h"
#include "Event.h"


namespace rz {
	class RZAPI WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height){
			// nothing fancy
		}

		static EventType GetStaticType() { return EventType::WindowResize; }
		virtual EventType GetEventType() const override { return GetStaticType();  }
		virtual const char* GetName() const override { return "WindowResize"; }
		virtual int GetCategoryFlags() const { return EventCategoryApplication; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << " " << m_Height;
			return ss.str();
		}

	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class RZAPI WindowCloseEvent : public Event {
	public:
		WindowCloseEvent(){}
		static EventType GetStaticType() { return EventType::WindowClose; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowClose"; }
		virtual int GetCategoryFlags() const { return EventCategoryApplication; }
	};


	class RZAPI AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		static EventType GetStaticType() { return EventType::AppTick; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "AppTick"; }
		virtual int GetCategoryFlags() const { return EventCategoryApplication; }
	};


	class RZAPI AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		static EventType GetStaticType() { return EventType::AppUpdate; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "AppUpdate"; }
		virtual int GetCategoryFlags() const { return EventCategoryApplication; }
	};


	class RZAPI AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}
		static EventType GetStaticType() { return EventType::AppRender; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "AppRender"; }
		virtual int GetCategoryFlags() const { return EventCategoryApplication; }
	};
}