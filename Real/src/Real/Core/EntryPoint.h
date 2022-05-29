#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern Real::Application *Real::createApplication();

int main(int argc, char **argv) {

	Real::Log::init();

	RE_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	auto app = Real::createApplication();
	RE_PROFILE_END_SESSION();

	RE_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->run();
	RE_PROFILE_END_SESSION();

	delete app;
}
#else
	#error "Unsupported (yet) platform"
#endif

