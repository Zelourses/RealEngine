#pragma once

#ifdef PLATFORM_WINDOWS

extern RealEngine::Application *RealEngine::createApplication();

int main(int argc, char **argv) {

	auto app = RealEngine::createApplication();
	app->run();

	delete app;
}
#endif

