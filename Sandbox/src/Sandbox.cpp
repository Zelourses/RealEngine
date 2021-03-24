#include <RealEngine.h>

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}

};

RealEngine::Application* RealEngine::createApplication() {
	return new Sandbox();
}
