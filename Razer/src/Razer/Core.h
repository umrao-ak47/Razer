#pragma once

#ifdef RZ_PLATFORM_WINDOWS
	#ifdef RZ_BUILD_DLL
		#define RZAPI __declspec(dllexport)
	#else
		# define RZAPI __declspec(dllimport)
	#endif
#else
	#error Currently supported only Windows
#endif
