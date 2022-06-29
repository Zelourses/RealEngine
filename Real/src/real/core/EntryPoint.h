#pragma once

#include "real/core/Application.h"

#ifdef RE_PLATFORM_WINDOWS

int main(int argc, char **argv) {

	Real::Log::init();

	RE_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	auto app = Real::createApplication(Real::Application::createFromCmd(argc, const_cast<const char**>(argv)));
	RE_PROFILE_END_SESSION();

	RE_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->run();
	RE_PROFILE_END_SESSION();

	delete app;
}
#else
	#error "Unsupported (yet) platform"
#endif

