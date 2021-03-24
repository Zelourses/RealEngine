#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern RealEngine::Application *RealEngine::createApplication();

int main(int argc, char **argv) {

	RealEngine::Log::init();
	RealEngine::Log::getCoreLogger()->warn("Test");
	RealEngine::Log::getClientLogger()->info("Test");

	auto app = RealEngine::createApplication();
	app->run();

	delete app;
}
#endif

