#pragma once

#ifdef RZ_PLATFORM_WINDOWS
	#ifdef RZ_DYNAMIC_LINK
		#ifdef RZ_BUILD_DLL
			#define RZAPI __declspec(dllexport)
		#else
			# define RZAPI __declspec(dllimport)
		#endif
	#else
		#define RZAPI
	#endif
#else
	#error Currently supported only Windows
#endif

#ifdef RZ_ENABLE_ASSERT
	#define RZ_ASSERT(x, ...)       { if(!(x)) {HZ_ERROR("Assertation Failed: {0}", __VA__ARGS__); __debugbreak(); }}
	#define RZ_CORE_ASSERT(x, ...)  { if(!(x)) {HZ_CORE_ERROR("Assertation Failed: {0}", __VA__ARGS__); __debugbreak(); }}
#else
	#define RZ_ASSERT(x, ...)     
	#define RZ_CORE_ASSERT(x, ...)
#endif



#define BIT(x) (1 << x)
