#pragma once

#include "Razer/Time.h"

namespace rz {
	class WindowsTimer : public Timer {
	public:
		WindowsTimer()
			: m_Time{0} {}

		~WindowsTimer() {}

		float GetTime() override;
	private:
		float m_Time;
	};
}
