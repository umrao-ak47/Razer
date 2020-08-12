#pragma once

#include "Core.h"

namespace rz {
	class RZAPI Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// To be implemented in client side
	Application* CreateApplication();
}


