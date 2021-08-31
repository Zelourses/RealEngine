#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

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
#define RE_CORE_TRACE(...)	::Real::Log::getCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)	::Real::Log::getCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)	::Real::Log::getCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)	::Real::Log::getCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_FATAL(...)	::Real::Log::getCoreLogger()->fatal(__VA_ARGS__)
//client log macros
#define RE_TRACE(...)	::Real::Log::getClientLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)	::Real::Log::getClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...)	::Real::Log::getClientLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)	::Real::Log::getClientLogger()->error(__VA_ARGS__)
#define RE_FATAL(...)	::Real::Log::getClientLogger()->fatal(__VA_ARGS__)
