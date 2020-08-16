#pragma once

#include "Razer/Core.h"

namespace rz {
	class RZAPI Timer {
	public:
		virtual ~Timer() {}

		virtual float GetTime() = 0;

		float GetTimeInSeconds() { return GetTime(); }
		float GetTimeInMiliSeconds() { return GetTime()*1000.0f; }

		static Timer* Get();
	};
}