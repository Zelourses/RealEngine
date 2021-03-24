#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace RealEngine {
	class RE_API Log {
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; };
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; };

	};
}
//core log macros
#define RE_CORE_TRACE(...)	::RealEngine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)	::RealEngine::Log::getCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)	::RealEngine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)	::RealEngine::Log::getCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_FATAL(...)	::RealEngine::Log::getCoreLogger()->fatal(__VA_ARGS__)
//client log macros
#define RE_TRACE(...)	::RealEngine::Log::getClientLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)	::RealEngine::Log::getClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...)	::RealEngine::Log::getClientLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)	::RealEngine::Log::getClientLogger()->error(__VA_ARGS__)
#define RE_FATAL(...)	::RealEngine::Log::getClientLogger()->fatal(__VA_ARGS__)
