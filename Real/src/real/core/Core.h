#pragma once
#include <memory>

#ifdef RE_DEBUG
	#define RE_ENABLE_ASSERTS

	#if defined(RE_PLATFORM_WINDOWS)
		#define RE_DEBUGBREAK() __debugbreak()
	#elif defined(RE_PLATFORM_LINUX)
		#include <signal.h>
		#define RE_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't suppoted"
	#endif
#endif

#define BIT_SHIFT(x) (1 << x)


#define RE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define RE_EXPAND_MACRO(x)	 x
#define RE_STRINGIFY(x)		 #x

// One static assert to rule the whole project
static_assert(sizeof(unsigned int) == sizeof(uint32_t));

namespace Real {

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> createScope(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	constexpr Ref<T> createRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "debug/Assert.h"
