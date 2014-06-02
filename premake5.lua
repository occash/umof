solution "metasystem"
	configurations { "Release", "Debug" }
	location ( "build" )
	
	project "metasystem"
		targetname "engine"
		language "C++"
		kind "ConsoleApp"
		includedirs { os.getenv('BOOST_ROOT') .. '/include' }
		
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