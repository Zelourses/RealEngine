#pragma once

#ifdef RE_PLATFORM_WINDOWS
	#ifdef RE_BUILD_DLL
		#define RE_API __declspec(dllexport)
	#else
		#define RE_API __declspec(dllimport)
	#endif
#else
	#error Engine supports only Windows on that moment
#endif 

#ifdef RE_ENABLE_ASSERTS
	#define RE_ASSERT(x, ...) {if (!(x)){RE_ERROR("Assertion failed: {0}",__VA_ARGS__); __debugbreak();}}
	#define RE_CORE_ASSERT(x, ...) {if (!(x)){RE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define RE_ASSERT(x, ...)
	#define RE_CORE_ASSERT(x, ...)
#endif

#define BIT_SHIFT(x) (1<<x)

#define RE_BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)