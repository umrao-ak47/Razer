workspace "Razer"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
IncludeDir = {}
IncludeDir["GLFW"] = "Razer/external/GLFW/include"
IncludeDir["Glad"] = "Razer/external/Glad/include"
IncludeDir["imgui"] = "Razer/external/imgui"
IncludeDir["glm"] = "Razer/external/glm"

include "Razer/external/Glad"

project "GLFW"
	location "Razer/external/GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		'%{prj.location}/include/GLFW/glfw3.h',
		'%{prj.location}/include/GLFW/glfw3native.h',
		'%{prj.location}/src/context.c',
		'%{prj.location}/src/init.c',
		'%{prj.location}/src/input.c',
		'%{prj.location}/src/monitor.c',
		'%{prj.location}/src/vulkan.c',
		'%{prj.location}/src/window.c'
	}

	filter "system:windows"
		systemversion "latest"

		files
		{
			'%{prj.location}/src/win32_init.c',
			'%{prj.location}/src/win32_joystick.c',
			'%{prj.location}/src/win32_monitor.c',
			'%{prj.location}/src/win32_time.c',
			'%{prj.location}/src/win32_thread.c',
			'%{prj.location}/src/win32_window.c',
			'%{prj.location}/src/wgl_context.c',
			'%{prj.location}/src/egl_context.c',
			'%{prj.location}/src/osmesa_context.c'
		}

		defines
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "Imgui"
	location "Razer/external/imgui"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		'%{prj.location}/imconfig.h',
		'%{prj.location}/imgui.h',
		'%{prj.location}/imgui_internal.h',
		'%{prj.location}/imstb_rectpack.h',
		'%{prj.location}/imstb_textedit.h',
		'%{prj.location}/imstb_truetype.h',
		'%{prj.location}/imgui.cpp',
		'%{prj.location}/imgui_demo.cpp',
		'%{prj.location}/imgui_draw.cpp',
		'%{prj.location}/imgui_widgets.cpp'
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


project "Razer"
	location "Razer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "RZPCH.h"
	pchsource "Razer/src/Razer/RZPCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/external/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RZ_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Razer/external/spdlog/include",
		"Razer/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Razer"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RZ_DIST"
		runtime "Release"
		optimize "on"