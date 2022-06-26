#pragma once

#include "Log.h"
#include "real/core/Core.h"
#include <filesystem>

#ifdef RE_ENABLE_ASSERTS

	#define RE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
		{                                                  \
			if (!(check)) {                                \
				RE##typeCRITICAL(msg, __VA_ARGS__);        \
				RE_DEBUGBRAK();                            \
			}                                              \
		}

	#define RE_INTERNAL_ASSERT_WITH_MSG(type, check, ...)
	#define RE_INTERNAL_ASSERT_NO_MSG(type, check)

	#define RE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define RE_INTERNAL_ASSERT_GET_MACRO(...)						  RE_EXPAND_MACRO(RE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RE_INTERNAL_ASSERT_WITH_MSG, RE_INTERNAL_ASSERT_NO_MSG))

	#define RE_ASSERT(...)		RE_EXPAND_MACRO(RE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define RE_CORE_ASSERT(...) RE_EXPAND_MACRO(RE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define RE_ASSERT(...)
	#define RE_CORE_ASSERT(...)
#endif