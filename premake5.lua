workspace "bigint"
	configurations { "debug", "release" }

project "bigint"
	kind "SharedLib"
	language "C"
	targetdir "lib"

	includedirs { "include" }

	files { "include/bigint/*.h", "src/bigint/*.c" }

	defines "BIGINT_NULL_ASSERT"

	filter "configurations:debug"
		defines "BIGINT_DEBUG"
		symbols "On"
	filter "configurations:release"
		defines "BIGINT_RELEASE"
		symbols "On"
