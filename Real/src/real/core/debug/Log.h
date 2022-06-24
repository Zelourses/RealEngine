#pragma once

#include "real/core/Core.h"
//spdlog for now (commit hash a26e174b367a60a51f0be83a81d6d066330cdd8f) has some
// warnings about deprecated functions.
// And now this is the only way to turn it off
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Real {
	class Log {
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; };
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; };

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}
//core log macros
#define RE_CORE_TRACE(...)	  ::Real::Log::getCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)	  ::Real::Log::getCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)	  ::Real::Log::getCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)	  ::Real::Log::getCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_CRITICAL(...) ::Real::Log::getCoreLogger()->critical(__VA_ARGS__)
//client log macros
#define RE_TRACE(...)	 ::Real::Log::getClientLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)	 ::Real::Log::getClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...)	 ::Real::Log::getClientLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)	 ::Real::Log::getClientLogger()->error(__VA_ARGS__)
#define RE_CRITICAL(...) ::Real::Log::getClientLogger()->critical(__VA_ARGS__)
