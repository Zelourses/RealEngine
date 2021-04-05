#include "repch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace RealEngine {
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;

	void Log::init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		coreLogger = spdlog::stdout_color_mt("REALENGINE");
		coreLogger->set_level(spdlog::level::trace);

		clientLogger= spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);
	}
}
