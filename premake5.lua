solution "metasystem"
	configurations { "Release", "Debug" }
	location ( "build" )
	
	project "umof"
		targetname "umof"
		language "C++"
		kind "SharedLib"
		
		files
		{
			"src/**.h",
			"src/**.cpp"
		}
		
		configuration "Debug"
			targetdir "bin/debug"
			defines { "_DEBUG", "UMOF_LIBRARY" }
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines { "NDEBUG", "UMOF_LIBRARY" }
			optimize "On"
			
	project "test"
		targetname "umof_test"
		language "C++"
		kind "ConsoleApp"
		
		files
		{
			"test/**.h",
			"test/**.cpp"
		}
		
		includedirs { "src" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			libdirs { "bin/debug" }
			links { "umof" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			libdirs { "bin/release" }
			links { "umof" }