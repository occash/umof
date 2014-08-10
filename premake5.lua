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
			defines "_DEBUG"
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
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
		
		links { "umof" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"