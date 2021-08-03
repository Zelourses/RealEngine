#pragma once
#include <memory>

#ifdef RE_PLATFORM_WINDOWS
#else
	#error Engine supports only Windows on that moment
#endif 

#ifdef RE_DEBUG
	#define RE_ENABLE_ASSERTS
#endif

#ifdef RE_ENABLE_ASSERTS
	#define RE_ASSERT(x, ...) {if (!(x)){RE_ERROR("Assertion failed: {0}",__VA_ARGS__); __debugbreak();}}
	#define RE_CORE_ASSERT(x, ...) {if (!(x)){RE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define RE_ASSERT(x, ...)
	#define RE_CORE_ASSERT(x, ...)
#endif

#define BIT_SHIFT(x) (1<<x)

//maybe I need to rewrite it in lambda thing
/*
 * RE_BIND_EVENT_FN(Application::onEvent) ->
 * [this](auto&& PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); }
 *
 * ^ is an example of transforming into lambda
 * But [this] ... I am not sure if it's ok
 */
#define RE_BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)

namespace Real {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
}
