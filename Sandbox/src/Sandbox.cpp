#include <RealEngine.h>

class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"){}

	void onUpdate() override {
		if (RealEngine::Input::isKeyPressed(RE_KEY_TAB))
			RE_TRACE("Tab button was pressed!");
	}

	void onEvent(RealEngine::Event& event) override {
		//RE_TRACE("{0}", event);
		if (event.getEventType() == RealEngine::EventType::KeyPressed) {
			RealEngine::KeyPressedEvent& e = static_cast<RealEngine::KeyPressedEvent&>(event);
			RE_TRACE("{0}", static_cast<char>(e.getKeyCode()));
		}
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
