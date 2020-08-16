project "stb_image"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("../../../bin/" .. outputdir .. "%{prj.name}")
	objdir ("../../../bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		'stb_image.h',
		'stb_image.c'
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"