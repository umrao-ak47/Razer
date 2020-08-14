#pragma once

#include "Razer/Core.h"
#include "Razer/Input.h"

namespace rz {
	class WindowsInput : public Input {
	protected:
		bool IsKeyPressedImpl(int keyCode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		std::pair<float, float> GetMousePosImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};
}