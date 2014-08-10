include "qt.lua"
local qt = premake.extensions.qt

solution "benchmark"
	configurations { "Release", "Debug" }
	location ( "build" )
	
	project "bench_native"
		targetname "bench_native"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_native.cpp" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			
	project "bench_umof"
		targetname "bench_umof"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_umof.cpp" }
		links { "umof" }
		includedirs { "../../src" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			libdirs { "../../bin/debug" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			libdirs { "../../bin/release" }
			
	project "bench_camp"
		targetname "bench_camp"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_camp.cpp" }
		includedirs { "D:/third-party/camp-master/include", os.getenv("BOOST_ROOT") }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			-- Oops, get rid of absolute path in future
			libdirs { "D:/third-party/camp-master/Debug" }
			links { "campd" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			libdirs { "D:/third-party/camp-master/Release" }
			links { "camp" }
			
	project "bench_qt"
		targetname "bench_qt"
		language "C++"
		kind "ConsoleApp"
		
		qt.enable()
		-- Set local Qt path
		qtpath "D:/third-party/Qt5.3.1/5.3/msvc2013"
		qtmodules { "core" }
		qtprefix "Qt5"
		
		files {	"qt_test.h", "bench_qt.cpp" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"