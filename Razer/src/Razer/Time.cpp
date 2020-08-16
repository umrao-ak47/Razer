#include "RZPCH.h"
#include "Time.h"

#include "Platform/Windows/WindowsTime.h"

namespace rz {
	Timer* Timer::Get() {
		return new WindowsTimer();
	}
}