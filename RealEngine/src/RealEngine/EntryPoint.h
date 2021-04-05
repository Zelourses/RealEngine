#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern RealEngine::Application *RealEngine::createApplication();

int main(int argc, char **argv) {

	RealEngine::Log::init();

	auto app = RealEngine::createApplication();
	app->run();

	delete app;
}
#endif

