#include <RealEngine.h>

class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"){}

	void onUpdate() override {
		RE_INFO("ExampleLayer: update");
	}

	void onEvent(RealEngine::Event& event) override {
		RE_TRACE("{0}", event);
	}
};

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}

};

RealEngine::Application* RealEngine::createApplication() {
	return new Sandbox();
}
