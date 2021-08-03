#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern Real::Application *Real::createApplication();

int main(int argc, char **argv) {

	Real::Log::init();

	auto app = Real::createApplication();
	app->run();

	delete app;
}
#endif

