#pragma once

#ifdef RZ_PLATFORM_WINDOWS

extern rz::Application* rz::CreateApplication();

int main(int argc, char** argv) {
	auto app = rz::CreateApplication();
	app->Run();
	delete app;
}
#endif