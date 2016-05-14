workspace "Game"
	configurations { "Debug", "Release" }
	platforms { "Win32" }
	location "projects"

	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter { "platforms:Win32" }
		system "Windows"
		architecture "x32"

	filter { "platforms:Win64" }
		system "Windows"
		architecture "x64"

	filter {}

project "Game"
	kind "ConsoleApp"
	language "C++"
	location "projects"
	targetdir "build"

	files {"include/**.h", "src/**.cpp"}

	filter "configurations:Debug"
		targetdir "build/bin/debug"
		debugdir "build/bin/debug"

		links {
			"sfml-graphics-d",
			"sfml-window-d",
			"sfml-system-d",
			"sfml-audio-d",
			"sfml-network-d",
		}

	filter "configurations:Release"
		targetdir "build/bin/release"
		debugdir "build/bin/release"

		links {
			"sfml-graphics",
			"sfml-window",
			"sfml-system",
			"sfml-audio",
			"sfml-network",
		}

	filter { "platforms:Win32" }
		libdirs { "contrib/SFML-2.3.2/lib" }

	filter {}

	includedirs { "include" }

	links {
		"sfml-graphics",
		"sfml-window",
		"sfml-system",
		"sfml-audio",
		"sfml-network",
	}

	flags {
		"FatalCompileWarnings",
		"FatalLinkWarnings",
		"FatalWarnings",
		"MultiProcessorCompile"
}
