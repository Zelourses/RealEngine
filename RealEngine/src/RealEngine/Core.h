#pragma once

#ifdef PLATFORM_WINDOWS
	#ifdef BUILD_DLL
		#define RE_API __declspec(dllexport)
	#else
		#define RE_API __declspec(dllimport)
	#endif
#else
	#error Engine supports only Windows on that moment
#endif 
