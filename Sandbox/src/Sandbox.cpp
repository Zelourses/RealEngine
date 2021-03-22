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

int main() {
	Sandbox* sandbox = new Sandbox();
	sandbox->run();

	delete sandbox;

	return 0;
}