#pragma once

#ifdef RZ_PLATFORM_WINDOWS

extern rz::Application* rz::CreateApplication();

int main(int argc, char** argv) {
	rz::Log::init();
	RZ_CORE_WARN("Engine Initialized");
	int a = 5;
	RZ_INFO("Hello Var-{0}", a);

	auto app = rz::CreateApplication();
	app->Run();
	delete app;
}
#endif