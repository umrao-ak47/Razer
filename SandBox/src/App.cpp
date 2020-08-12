#include "Razer.h"

class App : public rz::Application {
public:
	App() {

	}
	~App() {

	}
};

rz::Application* rz::CreateApplication() {
	return new App();
}