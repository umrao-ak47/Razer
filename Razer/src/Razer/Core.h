#pragma once

#include <memory>

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

// for developement purpose
#define RZ_ENABLE_ASSERT

// Asserts 
#ifdef RZ_ENABLE_ASSERT
#define RZ_ASSERT(x, ...)       { if(!(x)) {RZ_ERROR("Assertation Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#define RZ_CORE_ASSERT(x, ...)  { if(!(x)) {RZ_CORE_ERROR("Assertation Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
#define RZ_ASSERT(x, ...)     
#define RZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

namespace rz{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;
}